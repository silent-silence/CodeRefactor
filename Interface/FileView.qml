import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id:fileView
    signal refactorClicked(var flag)

    Item {
        width: 0.45*parent.width
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: 0.02*width

        TextDisplayView{
            id:textCode
            width: 0.98*parent.width
            height: 0.98*parent.height
        }
        Text{
            id:textCodePath
            width: 0.98*parent.width
            height: 0.02*parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0.02*width
            anchors.bottom: textCode.top
        }
    }

    Button{
        id:refactorButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: "开始重构 >>"
        onClicked: {
            refactorClicked(checked)
        }
    }

    TextDisplayView{
        id:refactorCode
        width: 0.45*parent.width
        height: 0.98*parent.height
        anchors.right: parent.right
        anchors.rightMargin: 0.02*width
    }

    function setLeftView(path ,data){
        textCodePath.text=path
        textCode.text=data
    }
    function getLeftView(){
        return textCode.text
    }
    function setRightView(data){
        refactorCode.text=data
    }
    function getRightView(){
        return refactorCode.text
    }
}
