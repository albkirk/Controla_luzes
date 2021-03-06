#define discovery_prefix "homeassistant"

//JSON Variables
DynamicJsonDocument device_doc(128);
DynamicJsonDocument attributes_doc(128);
StaticJsonDocument<512> discovery_doc;
char device_jsonString[128];
char attributes_jsonString[128];
char discovery_jsonString[512];
                                

String type[] =    {"button_short_press", "button_short_release", "button_long_press", "button_long_release", "button_double_press", "button_triple_press", "button_quadruple_press", "button_quintuple_press"};
String payload[] = {"1", "0", "Long-ON", "Long-OFF", "2", "3", "4", "5"};
#define type_size 8

void hassio_device() {
    // device":{"name":"AmbiSense","identifiers":"09B376","manufacturer":"albroteam","model":"Ambiv2","sw_version":"01.01"}
    // Purge old JSON data and Load new values
    device_doc.clear();
    device_doc["name"]                  = config.DeviceName;                    // Device friendly name
    device_doc["ids"]                   = ChipID;                               // identifiers
    //device_doc["cns"] = String("['IP','" + WiFi.localIP().toString() + "']"); // connections
    device_doc["mf"]                    = BRANDName;                            // manufacturer
    device_doc["mdl"]                   = MODELName;                            // model
    device_doc["sw"]                    = SWVer;                                // sw_version
}

void trigger_discovery() {
    String mqtt_pathhassio = String(discovery_prefix) + "/";

    device_doc.clear();
    device_doc["ChipID"]                = ChipID;                               // Chip identifier
    device_doc["DeviceName"]            = config.DeviceName;                    // Device friendly name
    device_doc["Location"]              = config.Location;                      // Device Location
    device_doc["sw"]                    = SWVer;                                // sw_version

    serializeJson(device_doc, device_jsonString);                                //Serialize JSON data to string
    //telnet_println("Trigger Discovery: " + String(device_jsonString));
    mqtt_publish(mqtt_pathhassio, "Discovery", String(device_jsonString));
}

void send_attributes(String param) {
    serializeJson(attributes_doc, attributes_jsonString);                       //Serialize JSON data to string
    //telnet_println("HASSIO Attributes: " + String(attributes_jsonString));
    mqtt_publish(mqtt_pathtele, String("attr_" + param), String(attributes_jsonString));
}


void send_status_attributes(String param) {
    attributes_doc.clear();
    attributes_doc["IP"]                = WiFi.localIP().toString();            // Device IP address
    attributes_doc["Location"]          = config.Location;                      // Device Location
    attributes_doc["DEEPSleep"]         = config.DEEPSLEEP;                     // DEEPSleep status
    attributes_doc["SLEEPTime"]         = config.SLEEPTime;                     // DEEPSleep time
    attributes_doc["APMode"]            = config.APMode;                        // APMode status
    attributes_doc["WEB"]               = config.WEB;                           // WEB status
    //attributes_doc["Boot"]              = ESPWakeUpReason();                  // Boot Reason  //Crasha?!!
    send_attributes(param);
}

void send_switch_attributes(String param) {
    attributes_doc.clear();
    attributes_doc["auto_off_set"]      = TIMER;                                // auto_off_set
    attributes_doc["remaining_time"]    = TIMER_Current;                        // remaining_time
    send_attributes(param);
}

// HASSIO Configuration registration
void config_entity(String component = "sensor", String device_class = "Battery", String device = "") {
    //"sensor" have well defined classes: battery, humidity, illuminance, signal_strength, temperature, power, pressure, etc
    String param = "";
    if(device == "") param = device_class;       // use the "device_class" as "device" value
    else param = device;

    // Discovery Topic: // <discovery_prefix>/<component>/[<node_id>/]<object_id>/config
    String mqtt_pathhassio;
    if (param == "PowerPlug") mqtt_pathhassio = String(discovery_prefix) + "/" + component + '/' + ChipID.c_str() + '-' + device_class + '/';
    else mqtt_pathhassio = String(discovery_prefix) + "/" + component + '/' + ChipID.c_str() + '-' + param + '/';
    
    // Purge old JSON data and Load new values
    discovery_doc.clear();
    if (param == "PowerPlug") {
        discovery_doc["uniq_id"]        = ChipID + '-' + device_class;
        discovery_doc["name"]           = ChipID + ' ' + device_class;
    }
    else {
        discovery_doc["uniq_id"]        = ChipID + '-' + param;
        discovery_doc["name"]           = ChipID + ' ' + param;
    }

//    discovery_doc["name"]               = ChipID + ' ' + param;
//    if(device == "") discovery_doc["name"] = config.Location;
//    else discovery_doc["name"]          = config.Location + String(' ') + param;
    discovery_doc["dev"]                = device_doc;

    if(component == "cover") {
        discovery_doc["dev_cla"]        = device_class;                         // Device_class (ex.: shutter)
        discovery_doc["set_pos_t"]      = mqtt_pathconf + param;                // set_position_topic
        discovery_doc["pos_t"]          = mqtt_pathtele + param;                // position_topic
    }
    
    if(component == "switch") {
        discovery_doc["stat_t"]         = mqtt_pathtele + param;                // state_topic
        discovery_doc["cmd_t"]          = mqtt_pathconf + param;                // command_topic

        if(param == "LED") discovery_doc["icon"] = "hass:lighthouse-on";
        if(param == "Exterior") { 
            //discovery_doc["json_attr_t"] = mqtt_pathtele + "attr_" + param;     // Attributes topic
            discovery_doc["icon"] = "hass:coach-lamp";
        }
        if(param == "Interior") { 
            //discovery_doc["json_attr_t"] = mqtt_pathtele + "attr_" + param;     // Attributes topic
            discovery_doc["icon"] = "hass:ceiling-light";
        }
        if(param == "Socket") { 
            //discovery_doc["json_attr_t"] = mqtt_pathtele + "attr_" + param;     // Attributes topic
            discovery_doc["icon"] = "hass:power-socket-fr";
        }
        if(param == "Switch" || param == "Switch2") { 
            //discovery_doc["json_attr_t"] = mqtt_pathtele + "attr_" + param;     // Attributes topic
            discovery_doc["icon"] = "hass:power";
        }
        //discovery_doc["stat_off"]     = "0";
        //discovery_doc["stat_on"]      = "1";
        discovery_doc["pl_off"]         = "0";                                  // Payload_off
        discovery_doc["pl_on"]          = "1";                                  // Payload_on
    }


    if(component == "sensor") {
        if(device_class != "none") discovery_doc["device_class"] = device_class;
        discovery_doc["stat_t"]         = mqtt_pathtele + param;                // state_topic
    
        if(param == "Status") discovery_doc["json_attr_t"] = mqtt_pathtele + "attr_" + param;    // Attributes topic

        if(param == "Timer" || param == "Timer2") discovery_doc["icon"] = "hass:timer-outline";

        if(device_class == "Battery" || device_class == "Humidity" || device_class == "illuminance") {
            discovery_doc["unit_of_meas"] = "%";
            discovery_doc["val_tpl"]      = "{{ value | float }}";
        }

        if(device_class == "signal_strength") {
            discovery_doc["unit_of_meas"] = "dBm";
            discovery_doc["val_tpl"]      = "{{ value | float }}";
            //discovery_doc["icon"]         = "hass:wifi";
        }

        if(device_class == "Temperature") {
            discovery_doc["unit_of_meas"] = "ºC";
            discovery_doc["val_tpl"]      = "{{value | float }}";
        }

        if(device_class == "Power") {
            discovery_doc["unit_of_meas"] = "W";
            discovery_doc["val_tpl"]      = "{{value_json.Power | float }}";
            //discovery_doc["icon"]         = "hass:flash";
        }

        if(device_class == "Voltage") {
            discovery_doc["unit_of_meas"] = "V";
            discovery_doc["val_tpl"]      = "{{value_json.Voltage | float }}";
            //discovery_doc["icon"]         = "hass:sine-wave";
        }

        if(device_class == "Current") {
            discovery_doc["unit_of_meas"] = "A";
            discovery_doc["val_tpl"]      = "{{value_json.Current | float }}";
            //discovery_doc["icon"]         = "hass:triangle-wave";            
        }

        if(device_class == "Energy") {
            discovery_doc["unit_of_meas"] = "Wh";
            discovery_doc["val_tpl"]      = "{{value_json.Energy | float }}";
            discovery_doc["icon"]         = "hass:transmission-tower";
        }
    }

    serializeJson(discovery_doc, discovery_jsonString);                     //Serialize JSON data to string
    //telnet_println("HASSIO Config: " + String(discovery_jsonString));
    mqtt_publish(mqtt_pathhassio, "config", String(discovery_jsonString), true);
}


void delete_entity(String component = "sensor", String device_class = "Battery", String param = "") {
    if(param == "") param = device_class;       // use the "device_class" as "param" value

    String mqtt_pathhassio;
    if (param == "PowerPlug") mqtt_pathhassio = String(discovery_prefix) + "/" + component + '/' + ChipID.c_str() + '-' + device_class + '/';
    else mqtt_pathhassio = String(discovery_prefix) + "/" + component + '/' + ChipID.c_str() + '-' + param + '/';

    mqtt_publish(mqtt_pathhassio, "config", "", true);
}


void config_trigger(String subtype = "Button_1", String param = "") {
    // subtype => ["button_1", "button_2", "button_3", "button_4", "button_5", "button_6"]

    if(param == "") param = subtype;       // use the "device_class" as "device" value

    String mqtt_pathhassio;

    for (size_t i = 0; i < type_size; i++)
    {
        // <discovery_prefix>/device_automation/[<node_id>/]<object_id>/config
        mqtt_pathhassio = String(discovery_prefix) + "/" + "device_automation" + '/' + ChipID.c_str() + '/' + subtype + '-' + type[i] + '/';

    
        // Purge old JSON data and Load new values
        discovery_doc.clear();
        discovery_doc["dev"]                = device_doc;

        discovery_doc["atype"]              = "trigger";                            // automation_type
        discovery_doc["topic"]              = mqtt_pathtele + param;
        discovery_doc["type"]               = type[i];
        discovery_doc["subtype"]            = subtype;
        discovery_doc["payload"]            = payload[i];

        serializeJson(discovery_doc, discovery_jsonString);                     //Serialize JSON data to string
        //telnet_println("HASSIO Config: " + String(discovery_jsonString));
        mqtt_publish(mqtt_pathhassio, "config", String(discovery_jsonString), true);
        yield();
    }

}


void delete_trigger(String subtype = "Button_1", String param = "") {
    // subtype => ["button_1", "button_2", "button_3", "button_4", "button_5"]  "button_6" is reserved for BOOT message!
    String mqtt_pathhassio;

    if(param == "") param = subtype;       // use the "device_class" as "device" value

    for (size_t i = 0; i < type_size; i++)
    {
        // <discovery_prefix>/device_automation/[<node_id>/]<object_id>/config
        mqtt_pathhassio = String(discovery_prefix) + "/" + "device_automation" + '/' + ChipID.c_str() + '/' + subtype + '-' + type[i] + '/';

        mqtt_publish(mqtt_pathhassio, "config", "", true);
    }

}


void config_trigger_boot(String type = "button_6", String subtype = "turn_on", String param = "Boot") {

    // <discovery_prefix>/device_automation/[<node_id>/]<object_id>/config
    String mqtt_pathhassio = String(discovery_prefix) + "/" + "device_automation" + '/' + ChipID.c_str() + '/' + subtype + '-' + type + '/';

    // Purge old JSON data and Load new values
    discovery_doc.clear();
    discovery_doc["dev"]                = device_doc;
    discovery_doc["atype"]              = "trigger";                            // automation_type
    discovery_doc["topic"]              = mqtt_pathtele + param;
    discovery_doc["type"]               = type;
    discovery_doc["subtype"]            = subtype;
    //discovery_doc["payload"]            = payload[i];
    serializeJson(discovery_doc, discovery_jsonString);                     //Serialize JSON data to string
    //telnet_println("HASSIO Config: " + String(discovery_jsonString));
    mqtt_publish(mqtt_pathhassio, "config", String(discovery_jsonString), true);

}

void delete_trigger_boot(String type = "button_6", String subtype = "turn_on", String param = "Boot") {

    String mqtt_pathhassio = String(discovery_prefix) + "/" + "device_automation" + '/' + ChipID.c_str() + '/' + subtype + '-' + type + '/';

    mqtt_publish(mqtt_pathhassio, "config", "", true);
}


#include <custohassio.h>

void hassio_discovery() {
    hassio_device();
    config_entity("sensor","none", "Status");
    delay(10); send_status_attributes("Status");
    if (BattPowered) config_entity("sensor","Battery");
    else delete_entity("sensor","Battery");
    if (LED_ESP>=0) config_entity("switch", "none", "LED");
    config_entity("sensor","signal_strength", "RSSI");
    //config_trigger_boot();
    custo_hassio_disc();
    config.HASSIO_CFG = true;
}

void hassio_delete() {
    hassio_device();
    delete_entity("sensor","none", "Status");
    delete_entity("sensor","Battery");
    delete_entity("switch", "none", "LED");
    delete_entity("sensor","signal_strength", "RSSI");
    //delete_trigger_boot();
    custo_hassio_del();
    config.HASSIO_CFG = false;
}

void hassio_attributes() {
    delay(10);
    send_status_attributes("Status");
    custo_hassio_attr();
}
