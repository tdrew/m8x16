/*
 * Message Board
 * Creation    : 08/05/2020
 * Author      : Tim Drew
 * Filename    : m8x16.ino
 * Hardware    : Arduino Due
 * Description : Control application for 8x16 Monochrome Led Display Matrix
 *             :
 *             :
 *
 * History     :
 */


void IIC_start()
{
  digitalWrite(IIC_SCL,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,LOW);
  delayMicroseconds(3);
}

void IIC_send(unsigned char send_data)
{
  for(char i = 0;i < 8;i++)
  {
      digitalWrite(IIC_SCL,LOW);
      delayMicroseconds(3); 
      if(send_data & 0x01)
      {
        digitalWrite(IIC_SDA,HIGH);
      }
      else
      {
        digitalWrite(IIC_SDA,LOW);
      }
      delayMicroseconds(3);
      digitalWrite(IIC_SCL,HIGH); 
      delayMicroseconds(3);
      send_data = send_data >> 1;
  }
}

void IIC_end()
{
  digitalWrite(IIC_SCL,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,HIGH);
  delayMicroseconds(3);
}