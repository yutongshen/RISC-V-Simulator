#include "tmdl.h"
#include "mmap/tmdl_reg.h"
#include "util/util.h"

TMDL::TMDL(const char *path) : Slave(0x1000)
{
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof line, file) != NULL) {
            tmdl_log.push_back(line);
        }
        fclose(file);
    }
}

TMDL::~TMDL() {}

bool TMDL::write(const Addr &addr,
                 const DataType &data_type,
                 const uint64_t &wdata)
{
    if (addr == RG_TM_PRINT) {
        printf("% 6ld ns: [TMDL PRINT] ", *time);
        tm_print(tmdl_log[wdata].c_str());
    } else if (addr == RG_TM_ARGS) {
        arg_fifo[arg_head++] = (uint64_t) wdata;
        arg_head %= FIFO_DEPTH;
    } else if (addr >= RG_TM_FORCE_IRQ) {
        force_irq[(addr - RG_TM_FORCE_IRQ) >> 2] = wdata;
    }
    return 1;
}

bool TMDL::read(const Addr &addr, const DataType &data_type, uint64_t &rdata)
{
    rdata = 0;
    if (addr >= RG_TM_FORCE_IRQ) {
        rdata = force_irq[(addr - RG_TM_FORCE_IRQ) >> 2];
    }

    return 1;
}

void TMDL::tm_print(const char *fmt)
{
    char str[128];
    char label[8];
    char *str_ptr(str);
    char *label_ptr(label);
    char qualifier;

    for (; *fmt; ++fmt) {
        label_ptr = label;
        if (*fmt != '%') {
            if (*fmt == '\n')
                *(str_ptr++) = '\r';
            *(str_ptr++) = *fmt;
            continue;
        } else {
            *str_ptr = '\0';
            printf("%s", str);
            str_ptr = str;
            ++fmt;
        }
        *(label_ptr++) = '%';
    BUILD_LABEL:
        *(label_ptr++) = *fmt;
        switch (*fmt) {
        case '\0':
            return;
        case '%':
            printf("%%");
            continue;
        case 'l':
        case 'h':
            qualifier = *(fmt++);
            goto BUILD_LABEL;
        case 's':
        case 'n':
            printf("[!! TMODEL CANNOT SUPPORT %%%c !!]", *fmt);
            break;
        case 'p':
            *label_ptr = '\0';
            printf(label, (void *) arg_fifo[arg_tail]);
            break;
        case 'c':
            *label_ptr = '\0';
            printf(label, (uint8_t) arg_fifo[arg_tail]);
            break;
        case 'd':
        case 'i':
        case 'u':
        case 'o':
        case 'x':
        case 'X':
            *label_ptr = '\0';
            if (qualifier == 'l')
                printf(label, (uint64_t) arg_fifo[arg_tail]);
            else if (qualifier == 'h')
                printf(label, (uint16_t) arg_fifo[arg_tail]);
            else
                printf(label, (uint32_t) arg_fifo[arg_tail]);
            break;
        case 'f': {
            *label_ptr = '\0';
            union uint32_float32 {
                uint32_t ui;
                float f;
            } _value;
            _value.ui = arg_fifo[arg_tail];
            printf(label, _value.f);
        } break;
        default:
            ++fmt;
            goto BUILD_LABEL;
        }
        ++arg_tail;
        arg_tail %= FIFO_DEPTH;
    }
    // *str_ptr++ = '\r';
    // *str_ptr++ = '\n';
    *str_ptr = '\0';
    printf("%s", str);
}

void TMDL::set_time(const uint64_t *time)
{
    this->time = time;
}

void TMDL::bind_irq(uint32_t *irq)
{
    force_irq = irq;
};
