import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

//Slide component for displaying code content and line number.
Item{
    id: codeView

    //Code font size.
    readonly property int font_size: 0.014*codeContrast.height

    //Interface component spacing.
    readonly property real spacing_size: 2

    //Controls the order in which line numbers and text
    //content are displayed (left to right by default).
    property bool negative_direction: false

    property alias text: code_content.text

    Rectangle{
        id: frame
        anchors.fill: parent
        border.width: spacing_size
        radius: spacing_size*2
    }

    Row{
        id: row1
        anchors.fill: parent
        MouseArea{
            width: parent.width-scrollBar.width
            height: parent.height
            onWheel: {
                scrollBar.active = true
                if(scrollBar.size < 1 && scrollBar.size > 0){
                    var rate = scrollBar.size * 0.1
                    var delta = wheel.angleDelta.y/120
                    switch(delta){
                    case -1:
                        if(scrollBar.position > 1-scrollBar.size-rate)
                            scrollBar.position = 1-scrollBar.size
                        else
                            scrollBar.position +=  rate
                        break;
                    case 1:
                        if(scrollBar.position < rate)
                            scrollBar.position = 0
                        else
                            scrollBar.position -= rate
                        break;
                    default:
                        console.log("delta: "+delta)
                        break;
                    }
                }
                scrollBar.active = false
            }
        }
        ScrollBar{
            id: scrollBar
            height: parent.height
            hoverEnabled: true
            active: hovered || pressed
            orientation: Qt.Vertical
            size: (parent.height / code_content.height).toFixed(2)
            onPositionChanged: {
                code_content.y = parent.y - code_content.height * position
                line_number.y = parent.y - line_number.height * position
            }
        }
    }

    Row{
        id: row2
        anchors.fill: parent
        clip: true
        Text{
            id: line_number
            width: parent.width*0.08
            anchors.margins: spacing_size
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: font_size
            Rectangle{
                id: line_color
                anchors.fill: parent
                anchors.margins: spacing_size
                radius: spacing_size*2
                color: "#66000000"
            }
        }
        Text{
            id: code_content
            width: parent.width*0.9
            anchors.margins: spacing_size
            font.pointSize: font_size
            horizontalAlignment : Text.AlignLeft
            onTextChanged: {
                line_number.text = getLineStr(code_content.lineCount)
            }
        }

    }

    onNegative_directionChanged: {
        if(negative_direction){
            row1.layoutDirection = Qt.RightToLeft
            row2.layoutDirection = Qt.RightToLeft
        }
        else{
            row1.layoutDirection = Qt.RightToLeft
            row2.layoutDirection = Qt.LeftToRight
        }
    }

    function getLineStr(number){
        var str=""
        for(var i=1; i<number; i++){
            str+=i+"\n"
        }
        str+=number
        return str
    }
}
