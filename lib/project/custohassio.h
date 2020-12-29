// Function to insert customized HASSIO Configuration actions

void custo_hassio_disc(){
    config_entity("switch","Exterior");
    config_entity("switch","Interior");
    config_entity("sensor","none","Timer");
    config_entity("sensor","none","Timer2");
    config_trigger("Button_1", "Exterior");
    config_trigger("Button_2", "Interior");
}

void custo_hassio_del(){
    delete_entity("switch","Exterior");
    delete_entity("switch","Interior");
    delete_entity("sensor","none","Timer");
    delete_entity("sensor","none","Timer2");
    delete_trigger("Button_1", "Exterior");
    delete_trigger("Button_2", "Interior");
}

void custo_hassio_attr(){
    //send_switch_attributes("Exterior");
    //send_switch_attributes("Interior");
}
