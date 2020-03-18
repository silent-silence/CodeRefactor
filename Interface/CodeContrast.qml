import QtQuick 2.14
import QtQuick.Window 2.14

Item {
    id: codeContrast

    //Font size.
    readonly property int font_size: 0.013*codeContrast.height

    //Interface component spacing.
    readonly property int spacing_size: 2

    property alias pathLeft: pathLeft.text
    property alias pathRight: pathRight.text
    property alias codeLeft: codeViewLeft.text
    property alias codeRight: codeViewRight.text

    signal refactorClicked()

    Grid{
        id: grid
        anchors.margins: spacing_size
        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignHCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        padding: spacing_size*2
        columns: 3

        Row {
            width: codeViewRight.width
            height: 0.04*codeContrast.height
            Text {
                id: textLeft
                anchors.leftMargin: spacing_size*2
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: font_size
                text: qsTr("Path: ")
            }
            Text{
                id: pathLeft
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: font_size
            }
        }

        Text {
            width: button.width
            height: 0.04*codeContrast.height
        }

        Row {
            width: codeViewRight.width
            height: 0.04*codeContrast.height
            Text {
                id: textRight
                anchors.leftMargin: spacing_size*2
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: font_size
                text: qsTr("Path: ")
            }
            Text{
                id: pathRight
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: font_size
            }
        }

        CodeView{
            id: codeViewLeft
            width: 0.44*codeContrast.width
            height: 0.95*codeContrast.height
            anchors.margins: spacing_size
            negative_direction: true
        }

        Rectangle{
            id: button
            width: 0.1*codeContrast.width
            height: 0.05*codeContrast.height
            anchors.margins: spacing_size
            radius: spacing_size*2
            color: "#66000000"
            Text{
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Start Refactoring\n>>>")
                font.pointSize: font_size
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    refactorClicked()
                }
            }
        }

        CodeView{
            id: codeViewRight
            width: 0.44*codeContrast.width
            height: 0.95*codeContrast.height
            anchors.margins: spacing_size
            negative_direction: false
        }
    }
}
