import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import CodeInterface 1.0

ApplicationWindow {
    visible: true
    minimumWidth: 960
    minimumHeight: 540
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    menuBar: MenuBarView{
        id: menuBarView
        onOpenFile: {
            codeRefactor.inputPath = path
        }
        onSaveAsFile: {
            var str=codeContrast.codeRight
            if(str===""){
                errorDialog.showError("The file is empty.")
            }
            else{
                codeRefactor.outputPath=path
                codeRefactor.writeOutputFileData()
            }
        }
        onSaveFile: {
            codeRefactor.outputPath=codeRefactor.inputPath
            codeRefactor.writeOutputFileData()
        }
        onLoopRefactor: {
            switch(value){
            case 0:
                codeRefactor.setFor_While(false)
                codeRefactor.setWhile_For(false)
                break
            case 1:
                codeRefactor.setFor_While(true)
                break
            case 2:
                codeRefactor.setWhile_For(true)
                break
            default:
                break
            }
        }
        onConditionRefactor: {
            switch(value){
            case 0:
                codeRefactor.setSCMIf_MCIf(false)
                codeRefactor.setMCIf_SCMIf(false)
                break
            case 1:
                codeRefactor.setMCIf_SCMIf(true)
                break
            case 2:
                codeRefactor.setSCMIf_MCIf(true)
                break
            default:
                break
            }
        }
        onSwitchRefactor: {
            switch(value){
            case 0:
                codeRefactor.setSwitch_MIf(false)
                codeRefactor.setMIf_Switch(false)
                break
            case 1:
                codeRefactor.setMIf_Switch(true)
                break
            case 2:
                codeRefactor.setSwitch_MIf(true)
                break
            default:
                break
            }
        }
        onRefactorName: {
            codeInterface.setRefactorName(value)
            console.error("refactor name " + value)
        }
        onDetectNullPointer: {
            codeInterface.setDetectNullPointer(value);
            console.error("NullPointer " + value)
        }
    }

    CodeContrast{
        id: codeContrast
        anchors.fill: parent
        onRefactorClicked:{
            codeRefactor.getRefactorData()
            codeRefactor.refreshData()
        }
    }

    Refactor{
        id: codeRefactor
        onLeftTextChanged:{
            codeContrast.pathLeft=inputPath
            codeContrast.codeLeft=leftText
        }
        onRightTextChanged:{
            codeContrast.codeRight=rightText
        }
        onParseError: {
            errorDialog.showError(e)
        }
        onTextRefreshData:{
            codeContrast.codeLeft=leftText
            codeContrast.pathLeft=inputPath
            codeContrast.codeRight=rightText
        }
    }

    ErrorDialog{
        id: errorDialog
    }

    DropArea{
        anchors.fill: parent
        onDropped: {
            codeContrast.inputPath=drop.urls[0]
        }
    }
}
