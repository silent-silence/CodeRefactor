import QtQuick 2.14
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14

Dialog {
    id:errorDialog
    title: qsTr("错误")
    visible: false
    contentItem: Rectangle {
        implicitWidth: 400
        implicitHeight: 120
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
    function showError(str){
        errorDialog.visible=true
        errorText.text=str
    }
}

