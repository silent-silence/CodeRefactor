import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.12

Dialog {
    id:errorDialog
    title: qsTr("错误")
    visible: false
    contentItem: Rectangle {
        implicitWidth: 400
        implicitHeight: 120
        Row {
            id: row
            anchors.fill: parent
            Text {
                id: errorText
                anchors.verticalCenterOffset: -20
                anchors.centerIn: parent
            }
            Button {
                text: qsTr("ok")
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                onClicked: {
                    errorDialog.visible = false
                    errorText.text=""
                }
            }
        }
    }
    function showError(str){
        errorDialog.visible=true
        errorText.text=str
    }
}

