import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

Item {
    id:textDisplayView
    property string text: ""
    property int font_size: Screen.desktopAvailableHeight*0.015

    Component.onCompleted: {
        textContent.text=""
    }

    Rectangle{
        id:rectangle
        width: 0.98*parent.width
        height: 0.98*parent.height;
        anchors.centerIn: parent
        radius: 0.01*width
        border.width: 1
        ScrollView {
            anchors.fill: parent
            padding: 0.01*width
            clip: true

            Text {
                id:lineNumber
                anchors.left: parent.left
                padding: parent.padding
                //activeFocusOnPress:false
                font.pixelSize: font_size
                Rectangle{
                    id:lineNumberBackground
                    height: lineNumber.height
                    radius: 0.1*width
                    color: "#66000000"
                }
                textFormat: TextArea.AutoText
            }
            Text {
                id:textContent
                anchors.left: parent.left
                leftPadding:lineNumber.font.weight
                wrapMode: TextArea.Wrap
                padding: parent.padding
                //activeFocusOnPress:false
                font.pixelSize: font_size
                textFormat: TextArea.AutoText
            }
        }
    }
    Connections{
        target: textContent

        onLineCountChanged: {
            lineNumber.text=getLineNumber(textContent.lineCount)
            lineNumberBackground.width=(textContent.lineCount.toString().length+1)*lineNumber.font.pixelSize
        }
        function getLineNumber(number){
            var str=""
            for(var i=0; i<number; i++){
                str+=i+1
                str+="\n"
            }
            return str
        }
    }
    Connections{
        target: lineNumberBackground
        onWidthChanged:{
            textContent.leftPadding=lineNumber.font.weight+lineNumberBackground.width
        }
    }

    onTextChanged: {
        textContent.text=text
    }
}
