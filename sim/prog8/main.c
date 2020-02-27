unsigned int *copy_addr; // = &_test_start;
unsigned int copy_count = 0;
const unsigned int sensor_size = 64;
volatile unsigned int *sensor_addr = (int *) 0x10000000;
/*****************************************************************
 * Function: void copy()                                         *
 * Description: Part of interrupt service routine (ISR).         *
 *              Copy data from sensor controller to data memory. *
 *****************************************************************/
void copy () {
  int i;
  for (i = 0; i < sensor_size; i++) { // Copy data from sensor controller to DM
    *(copy_addr + i) = sensor_addr[i];
  }
  copy_addr += sensor_size; // Update copy address
  copy_count++;    // Increase copy count
  sensor_addr[0x80] = 1; // Enable sctrl_clear
  sensor_addr[0x80] = 0; // Disable sctrl_clear
  if (copy_count == 4) {
    asm("li t6, 0x80");
    asm("csrc mstatus, t6"); // Disable MPIE of mstatus
  }
  return;
}

/*****************************************************************
 * Function: void sort(int *, unsigned int)                                    *
 * Description: Sorting data                                     *
 *****************************************************************/
void sort(int *array, unsigned int size) {
  int i, j;
  int temp;
  for (i = 0; i < size - 1; i++) {
    for (j = i + 1; j < size; j++) {
      if (array[i] > array[j]) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
      }
    }
  }

  return;
}

int main(void) {
  extern unsigned int _test_start;
  int *sort_addr = &_test_start;
  int sort_count = 0;
  copy_addr = &_test_start;

  // Enable Global Interrupt
  asm("csrsi mstatus, 0x8"); // MIE of mstatus

  // Enable Local Interrupt
  asm("li t6, 0x800");
  asm("csrs mie, t6"); // MEIE of mie 

  // Enable Sensor Controller
  sensor_addr[0x40] = 1; // Enable sctrl_en

  while (sort_count != 4) {
    if (sort_count == copy_count) { // Sensor controller isn't ready
      // Wait for interrupt of sensor controller
      asm("wfi");
    }

    // Start sorting
    sort(sort_addr, sensor_size);
    sort_addr += sensor_size;
    sort_count++;
  }

  return 0;
}
