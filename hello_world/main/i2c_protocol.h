#ifndef __I2C_PROTOCOL_H__
#define __I2C_PROTOCOL_H__

#define I2C_MASTER_SCL_IO 19               /*!< gpio number for I2C master clock */    // set the gpio num same to default idf sdk
#define I2C_MASTER_SDA_IO 18               /*!< gpio number for I2C master data  */    // set the gpio num same to default idf sdk
#define I2C_MASTER_NUM 1 /*!< I2C port number for master dev */    // get it from default idf sdk
#define I2C_MASTER_FREQ_HZ 1000        /*!< I2C master clock frequency */    // the deault value is 100000
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define ESP_SLAVE_ADDR 0x28 /*!< ESP32 slave address, you can set any 7bit value */    // get it from default idf sdk
#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

esp_err_t i2c_master_init(void);

uint8_t i2c_read_sensor_reg(uint8_t reg_addr,uint8_t *data_rd, size_t size);

#endif
