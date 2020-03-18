import QtQuick 2.14
import QtQuick.Controls 2.14

Menu{
    property alias model: repeater.model

    signal chooseChanged(var value)

    Repeater{
        id:repeater
        RadioButton{
            text: qsTr(modelData)
            checked: text === qsTr("No Conversion")
            onCheckedChanged: {
                if(checked)
                    chooseChanged(modelData)
            }
        }
    }
}
