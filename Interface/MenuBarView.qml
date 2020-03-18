import QtQuick 2.14
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14

MenuBar{
    id: menuBarView

    signal openFile(var path)
    signal saveFile()
    signal saveAsFile(var path)

    signal loopRefactor(var value);
    signal conditionRefactor(var value);
    signal switchRefactor(var value);

    signal refactorName(var value);
    signal detectNullPointer(var value);

    Menu{
        title: qsTr("&File")

        Repeater{
            id: repeater
            model: ["&Open...", "&Save", "Save &As..."]
            MenuItem{
                text: qsTr(modelData)
                onTriggered: {
                    fileDialog.menuItemClicked=repeater.model.indexOf(modelData)
                    fileDialog.open()
                }
            }
        }
        MenuSeparator{}
        Action{
            text: qsTr("&Quit")
            onTriggered: {
                Qt.quit()
            }
        }
    }

    Menu {
        title: qsTr("&Refactor")

        RadioMenu{
            title: qsTr("&Loop")
            model: ["No Conversion", "For->While", "While->For"]
            onChooseChanged: {
                loopRefactor(model.indexOf(value))
            }
        }
        RadioMenu{
            title: qsTr("&Condition")
            model: ["No Conversion", "Multi->Single", "Single->Multi"]
            onChooseChanged: {
                conditionRefactor(model.indexOf(value))
            }
        }
        RadioMenu{
            title: qsTr("&Switch")
            model: ["No Conversion", "If->Switch", "Switch->If"]
            onChooseChanged: {
                switchRefactor(model.indexOf(value))
            }
        }

        CheckBox{
            text: qsTr("Name Refactor")
            onCheckedChanged: {
                refactorName(checked)
            }
        }
        CheckBox{
            text: qsTr("Null Pointer Detection")
            onCheckedChanged: {
                detectNullPointer(checked)
            }
        }
    }

    Menu {
        title: qsTr("&Help")
        Action{
            text: qsTr("&About")
        }
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["*.c","*.h","*.cpp","*.hpp"]
        title: "Select File"
        folder: shortcuts.home
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        property int menuItemClicked :0

        onAccepted: {
            switch(menuItemClicked){
            case 0:
                openFile(fileUrl)
                break
            case 2:
                saveAsFile(fileUrl)
                break
            default:
                break
            }
            fileDialog.close()
        }

        onMenuItemClickedChanged: {
            switch(menuItemClicked){
            case 0:
                title=qsTr("Open File")
                selectExisting=true
                break
            case 2:
                title=qsTr("Save As...")
                selectExisting=false
                break
            default:
                break
            }
        }
    }
}

