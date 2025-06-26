# Group 3: ESP32 + Firebase 
Our group is responsible for creating an app that allows you to read temperature, altitute and pressure in real-time from a DHT11 sensor with ESP32 and send the data to Firebase to disply it on the app.
## Physical Setup
We will need a PC, a USB, connecting wires, ESP32 and BMP280

![image](https://github.com/user-attachments/assets/0287d313-0812-4fee-ae01-335911b4b4a8)

Following this diagram, we can connect the ESP32 and BMP280:
- Wire GND Pin (BMP280) to GND Pin (ESP32)
- Wire SDA Pin (BMP280) to P21 Pin (ESP32)
- Wire GND Pin (BMP280) to GND Pin (ESP32)
- Wire VDD Pin (BMP280) to 3v3 Pin (ESP32)

## Software Setup
### Create a Firebase Realtime-data
1. Create an google email account
2. Create a Firebase project 
3. Go to Authentication > Get started
4. Add a user by choosing Email/Password > Enable Email/Password > Add user with email and password
5. Go to project settings > Copy the API Key
6. Create Realtime Database 
