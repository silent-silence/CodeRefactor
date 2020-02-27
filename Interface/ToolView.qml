import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

Item {
    id:toolView

    signal openFileClicked(var path)
    signal saveAsClicked(var path)
    signal saveClicked()

    signal mcsifScmif(var value);
    signal scmifMcsif(var value);
    signal mifSwitch(var value);
    signal switchMIf(var value);
    signal forWhile(var value);
    signal whileFor(var value);
	signal refactorName(var value);
	signal detectNullPointer(var value);

    MenuBar {
        anchors.fill:parent
        Menu{
            id:file;
            title:qsTr("文件(&F)");
            MenuItem{
                text:qsTr("&打开文件");
                onClicked: {
                    fileDialog.menuItemClicked=1
                    fileDialog.open()
                }
            }
            MenuItem{
                text:qsTr("&保存");

                onClicked: {
                    fileDialog.menuItemClicked=2
                    saveClicked();
                }
                action: Action{
                    shortcut: "Ctrl+S"
                    onTriggered: {
                        fileDialog.menuItemClicked=2
                        saveClicked();
                    }
                }
            }
            MenuItem{
                text:qsTr("&另存为");
                onClicked: {
                    fileDialog.menuItemClicked=3
                    fileDialog.open()
                }
                action: Action{
                    shortcut: "Ctrl+Shift+S"
                    onTriggered: {
                        console.log(1)
                        fileDialog.menuItemClicked=3
                        fileDialog.open()
                    }
                }
            }
            MenuItem{
                text:qsTr("&退出")
                onClicked: Qt.quit();
            }
        }
        Menu{
            id:edit;
            title:qsTr("重构(&R)");
            Menu{
                title:"for/while互转";
                CheckBox{
                    id:none1
                    text: "不转换"
                    checked: !(forstmt.checked||whilestmt.checked)
                    onClicked: {
                        if(checked){
                            forWhile(false)
                            whileFor(false)
                        }
                    }
                }
                CheckBox{
                    id:forstmt
                    text: "for->while"
                    checked: !(whilestmt.checked||none1.checked)
                    onCheckedChanged:{
                        forWhile(checked)
                    }
                }
                CheckBox{
                    id:whilestmt
                    text: "while->for"
                    checked: !(forstmt.checked||none1.checked)
                    onCheckedChanged:{
                        whileFor(checked)
                    }
                }
            }
            Menu {
                title:"多条件单if/单条件多if互转";
                CheckBox{
                    id:none2
                    text: "不转换"
                    checked: !(scmif_mcsif.checked||mcsif_scmif.checked)
                    onCheckedChanged: {
                        if(checked){
                            mcsifScmif(false)
                            scmifMcsif(false)
                        }
                    }
                }
                CheckBox{
                    id:mcsif_scmif
                    text: "多条件单if->单条件多if"
                    checked: !(scmif_mcsif.checked||none2.checked)
                    onCheckedChanged: {
                        mcsifScmif(checked)
                    }
                }
                CheckBox{
                    id:scmif_mcsif
                    text: "单条件多if->多条件单if"
                    checked: !(mcsif_scmif.checked||none2.checked)
                    onCheckedChanged: {
                        scmifMcsif(checked)
                    }
                }
            }

            Menu{
                title:"多if/switch互转";
                CheckBox{
                    id:none3
                    text: "不转换"
                    checked: !(mif_switch.checked||switch_mif.checked)
                    onCheckedChanged: {
                        if(checked){
                            mifSwitch(false)
                            switchMIf(false)
                        }
                    }
                }
                CheckBox{
                    id:mif_switch
                    text: "多if->switch"
                    checked: !(switch_mif.checked||none3.checked)
                    onCheckedChanged: {
                        mifSwitch(checked)
                    }
                }
                CheckBox{
                    id:switch_mif
                    text: "switch->多if"
                    checked: !(mif_switch.checked||none3.checked)
                    onCheckedChanged: {
                        switchMIf(checked)
                    }
                }
            }

			MenuItem {
				CheckBox {
					id: refactor_name
					text: qsTr("命名重构")
					checked: false
					onCheckStateChanged: {
						refactorName(checked)
					}
				}
			}

			MenuItem {
				CheckBox {
					id: detect_null_pointer
					text: qsTr("空指针检测")
					checked: false
					onCheckStateChanged: {
						detectNullPointer(checked)
					}
				}
			}
        }
        Menu {
            title: qsTr("帮助(&H)")
            Action { text: qsTr("&About") }
        }
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["*.*","*.h","*.cpp","*.hpp"]
        title: "选择文件"
        folder: shortcuts.home
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        property int menuItemClicked :0

        onAccepted: {
            switch(menuItemClicked){
            case 1:openFileClicked(fileUrl);break;
            case 3:saveAsClicked(fileUrl);break;
            default:break;
            }
            fileDialog.close()
        }

        onMenuItemClickedChanged: {
            switch(menuItemClicked){
            case 1:
                title="打开文件";
                selectExisting=true
                break;
            case 3:
                title="另存为";
                selectExisting=false
                break;
            default:break;
            }
        }
    }
}
