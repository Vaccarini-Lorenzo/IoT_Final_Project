// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication
const uint8_t scl = D5;
const uint8_t sda = D7;

// sensitivity scale factor respective to full scale setting provided in datasheet
const uint16_t gyroScaleFactor = 131;

// Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
// Gyroscope Output Rate = 8kHz (DLPF disabled)
const uint8_t SMPLRT_DIV   =  0x19;

const uint8_t CONFIG = 0x1A;

// Enables DataReady Interrupts register
const uint8_t INT_ENABLE   =  0x38;

// Interrupt status register
const uint8_t INT_STATUS   =  0x3A;

// Power management register #1
const uint8_t PWR_MGMT_1   =  0x6B;

// Power management register #2
// Set axis stand-by
const uint8_t PWR_MGMT_2   =  0x6C;

// Configures gyroscope's full scale range
const uint8_t GYRO_CONFIG  =  0x1B;

// First of the 14 output register
const uint8_t FIRST_REG =  0x3B;

// Number of output registers
int OFFSET = 14;

// 1 if the MPU has collected a new data sample
bool dataReady;

// Monitor roll value (Z Axis inclination)
int16_t roll;

// Monitor nominal roll value (The initial roll)
int16_t nominalRoll;

// Monitor spatial displacement
bool isHorizontal = true;

void WriteByte(uint8_t deviceAddress, uint8_t regAddress, uint8_t data);
void initMPU();
void readStatus();
void readGyroZ();
void setNominalRoll();
bool moved();

// Write byte to specified address
void WriteByte(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

void initMPU(){
  delay(150);
  // Enable sleep cycle & disable temperature sensor
  WriteByte(MPU6050SlaveAddress, PWR_MGMT_1, 0x29);
  // Set stand-by & cycle duration
  WriteByte(MPU6050SlaveAddress, PWR_MGMT_2, 0x01);
  // Set sample divider to 128
  WriteByte(MPU6050SlaveAddress, SMPLRT_DIV, 0x7F);
  // Enables DATA_RDY interrupt
  WriteByte(MPU6050SlaveAddress, INT_ENABLE, 0x01);
  // Set +/-250 degree/second full scale
  WriteByte(MPU6050SlaveAddress, GYRO_CONFIG, 0x00);
  // Set the output frequency
  WriteByte(MPU6050SlaveAddress, CONFIG, 0x07);
}

// Read the INT_STATUS register
void readStatus(){
  Wire.beginTransmission(MPU6050SlaveAddress);
  Wire.write(INT_STATUS);
  Wire.endTransmission();
  Wire.requestFrom(MPU6050SlaveAddress, 1);
  byte status = Wire.read() && 0x1F;
  if (status && 0x01 == 0x01){
    dataReady = true;
  } else {
    dataReady = false;
  }
}

// Read the gyroscope Z axis value
void readGyroZ(){
  Wire.beginTransmission(MPU6050SlaveAddress);
  Wire.write(FIRST_REG);
  Wire.endTransmission();
  Wire.requestFrom(MPU6050SlaveAddress, (uint8_t)OFFSET);
  roll = (((int16_t)Wire.read()<<8) | Wire.read())/gyroScaleFactor;
}

// Set nominal inclination (the initial Roll)
void setNominalRoll(){
  readGyroZ();
  nominalRoll = roll;
}

bool moved(){
  if (isHorizontal) {
    if (roll > nominalRoll + 80) {
      isHorizontal = false;
      return true;
    }
  } else {
    if (roll < nominalRoll + 10) {
      isHorizontal = true;
      return true;
    }
  }
  return false;
}
