# PSA steering wheel adapter

### What is it ?
This is a project which aims to remote control your after market head unit in your car made by Peugeot or Citroen which uses VAN bus communication. This includes the following brands: Peugeot 206/307/406 and Citroen C2/C5.

This is a proof of concept so testing and contribution is very welcome.

### Project structure

In the **src** folder you can find an abstract class (*AbstractRemote.h*) for the remote functionalities. It defines a common interface for controlling the head units remotely. The other files inherit from this base class, and implement the vendor specific implementations as needed.  So after implementing your head unit remote class in the main sketch in the setup() method basically you just need to instantiate the desired remote control class and you are good to go. 

    AbstractRemote *remoteControl;
    remoteControl = new JVC_CNY173_Remote(OUT_PIN, LED_PIN);

### Supported head units
- JVC KD-330BT (thanks to Istvan Polacsek)

### Building the project

To build the project you will need the following:

 - Arduino IDE with ESP32 board support installed
 - Libraries installed from the Used libraries section

### Used libraries

- [ESP32 RMT peripheral VAN reader][lib_esp32_van_rx]

### Related links
- [JVC KD-330BT remote control by Istvan Polacsek][yt_jvc_kd330bt]

[lib_esp32_van_rx]: https://github.com/morcibacsi/esp32_rmt_van_rx/
[yt_jvc_kd330bt]: https://www.youtube.com/watch?v=8OANaTe5kxI

