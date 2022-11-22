const domElementNames = {
    mqttArea: "#mqtt-setup",
    mqttCheckbox: "#chkMQTT"
}

$(function(){
    $(domElementNames.mqttCheckbox).on("change", (e) => {
        if($(domElementNames.mqttCheckbox).is(":checked")){
            $(domElementNames.mqttArea).removeClass("d-none");
        }else{
            $(domElementNames.mqttArea).addClass("d-none");
        }
    })
    
});