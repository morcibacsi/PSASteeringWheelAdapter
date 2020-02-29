# PSA steering wheel adapter

### What is it ?
This is a project which aims to remote control your aftermarket head unit in your car made by Peugeot or Citroen. The project supports both CAN bus and VAN bus communication. A lot of brands should be supported, including Peugeot 206/307/406/407/308/508 and Citroen C2/C5/Xsara and many more.

This is a proof of concept so testing and contribution is very welcome. The remote controller codes where collected from the internet and I could not verify if they are working. However I believe this code is still a very good starting point if you want to control your aftermarket head unit in your car. In the related links I have collected the original source of the codes so if something is not working you can take a look on those as well, maybe I missed something.

### Project structure

In the **src** folder you can find an abstract class (*AbstractRemote.h*) for the remote functionalities. It defines a common interface for controlling the head units remotely. The other files inherit from this base class, and implement the vendor specific implementations as needed.  So after implementing your head unit remote class in the main sketch in the setup() method basically you just need to instantiate the desired remote control class and you are good to go. The same folder contains the abstract class for the remote source as well (*AbstractRemoteSource.h*). There are two implementations of this one for the VAN bus (*VanRemoteSource.h*) and one for the CAN bus (*CanRemoteSource.h*). Use one of the according to your vehicle type.

    // initializing a remote source (select this if your car has a VAN bus)
    AbstractRemoteSource* remoteSource;
    remoteSource = new VanRemoteSource(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN);
    
    // or select this if you car has a CAN bus
    canInterface = new CanMessageSenderEsp32Arduino(CAN_RX_PIN, CAN_TX_PIN);
    remoteSource = new CanRemoteSource(canInterface);

    // initializing a remote control for your aftermarket head unit
    AbstractRemote *remoteControl;
    remoteControl = new JVC_CNY173_Remote(OUT_PIN, LED_PIN);

### Supported head units

- JVC KD-330BT (thanks to Istvan Polacsek)
- Pioneer (thanks to George Lazarov)
- Alpine (thanks to Roman Jadrovski)
- Peugeot CAN "remote" 
    - Basically this forwards the incoming VAN message to the CAN bus so this way head units which does not support VAN bus but support Peugeot CAN bus protocol can respond to the remote stalk in cars with VAN bus. These are usually those fancy Android based head units which you can buy from China.

### Building the project

To build the project you will need the following:

 - Arduino IDE with ESP32 board support installed
 - Libraries installed from the Used libraries section

### Schematics

As mentioned above this is a proof of concept, so schematics is not yet available. You need to figure this out for yourself. However the related links should help you find the basics to have an idea. A hard requirement is an ESP32 board, others like ESP8266 or other Arduino boards won't work.  For a sample hardware to read the VAN or CAN bus you can check this repository: [VAN-CAN bridge][van_can_bridge_hw] this is a shield for the ESP32 dev board, and I successfully used it in my project.

### Used libraries

- [ESP32 RMT peripheral VAN reader][lib_esp32_van_rx]

### Related links

- [JVC KD-330BT remote control by Istvan Polacsek][yt_jvc_kd330bt]
- [Pioneer remote control by George Lazarov][yt_pioneer]
- [Alpine remote control by Roman Jadrovski][orig_alpine]

[lib_esp32_van_rx]: https://github.com/morcibacsi/esp32_rmt_van_rx/
[yt_jvc_kd330bt]: https://www.youtube.com/watch?v=8OANaTe5kxI
[yt_pioneer]: https://www.youtube.com/watch?v=RQmXv-3tn54
[orig_alpine]: https://github.com/jadrovski/alpine_control
[van_can_bridge_hw]: https://github.com/morcibacsi/PSAVanCanBridgeHW/tree/v1.1

### TODO
- Create a hardware which supports all the remotes above (contributions are very welcome)
