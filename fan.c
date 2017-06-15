#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ExportPin(int pin) {
  // open export 
  int fd = open("/sys/class/gpio/export", O_WRONLY);
  if(fd == -1) {
    fprintf(stderr, "Failed to export GPIO!\n");
    return 0;
  }

  // write pin
  char buff[8];
  int size = sprintf(buff, "%d", pin);
  write(fd, buff, size);
  close(fd);
  return 1;
}

// 0 == in, 1 == out
#define IN 0
#define OUT 1
int SetPinDirection(int pin, int dir) {
  char path[1024];
  sprintf(path, "/sys/class/gpio/gpio%d/direction", pin);
  int fd = open(path, O_WRONLY);
  if(fd == -1) {
    fprintf(stderr, "Failed to open %s for writing!\n", path);
    return 0;
  }

  // set direction
  if(write(fd, dir == IN ? "in" : "out", dir == IN ? 2 : 3) == -1) {
    fprintf(stderr, "Failed to set GPIO direction!\n");
    return 0;
  }

  close(fd);
  return 1;
}

int WritePin(int pin, int value) {
  char path[1024];
  sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(path, O_WRONLY);
  if(fd == -1) {
    fprintf(stderr, "Failed to open %s!\n", path);
    return 0;
  }

  // write value
  if(write(fd, value ? "1" : "0", 1) == -1) {
    fprintf(stderr, "Failed to write GPIO value!\n");
    return 0;
  }

  close(fd);
  
  return 1;
}

// Return temperature
float GetTemperature() {
  FILE *f = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
  if(f == NULL) {
    fprintf(stderr, "Failed to open temperature file!\n");
    return 0.0f; // that might be too cold???
  }

  // read temp
  double temp;
  fscanf(f, "%lf", &temp);
  temp *= 0.001;

  fclose(f);
  
  return temp;
}

int main(int argc, char *argv[]) {
  // check args
  if(argc < 4) {
    printf("Usage : %s [--test | --daemon ] fan_pin [pin_value] [min_temp max_temp]\n", argv[0]);
    return 1;
  }
  
  // test mode
  if(!strcmp(argv[1], "--test")) {
    // export pin
    int fanPin = atoi(argv[2]);
    ExportPin(fanPin);
    SetPinDirection(fanPin, OUT);
    WritePin(fanPin, atoi(argv[3]));

    return 0;
  }

  else if(!strcmp(argv[1], "--daemon")) {
    if(argc < 5) {
      printf("Usage : %s --daemon fan_pin min_temp max_temp\n", argv[0]);
      return 1;
    }
    
    // setup pint
    int fanPin = atoi(argv[2]);
    int state = 0; // fan off
    ExportPin(fanPin);
    SetPinDirection(fanPin, OUT);
    WritePin(fanPin, state);
    
    float minTemp = atof(argv[3]);
    float maxTemp = atof(argv[4]);
    
    // and loop until...
    int run = 1;
    while(run) {
      // read temp
      float temp = GetTemperature();

      // turn on
      if(!state && temp > maxTemp) {
	state = 1;
	WritePin(fanPin, state);
	//printf("Fan On, temp = %.1f\n", temp);
      }

      else if(state && temp < minTemp) {
	state = 0;
	WritePin(fanPin, state);
	//printf("Fan Off, temp = .1f\n", temp);
      }

      // uncomment for logging
      //printf("temp = %.1f, fan %s\n", temp, state ? "On" : "Off");
      sleep(1);
    }
    
  }

  // else, print usage
  printf("Usage : %s [--test | --daemon ] fan_pin [pin_value] [min temp max_temp]\n", argv[0]);
  return 1;
}
