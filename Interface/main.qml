import QtQuick 2.12
import QtQuick.Controls 2.12
import CodeInterface 1.0

ApplicationWindow{
	id:root
	visible: true
	width: 1280
	height: 720

	menuBar: ToolView{
		id: toolView
		width: parent.width
	}

	FileView{
		id:textDisplayView
		width: parent.width
		height: 0.9*parent.height
		anchors.bottom: parent.bottom
	}

	Interface{
		id:codeInterface
	}

    ErrorDialog{
        id: errorDialog
    }

    Connections{
        target: toolView
        onOpenFileClicked:{
            codeInterface.inputPath=path
        }
        onSaveAsClicked:{
            var str=textDisplayView.getRightView()
            if(str===""){
                errorDialog.showError("文件为空")
            }
            else{
                codeInterface.outputPath=path
                codeInterface.writeOutputFileData(str)
            }
        }
        onSaveClicked:{
            codeInterface.outputPath=codeInterface.inputPath
            codeInterface.writeOutputFileData(textDisplayView.getRightView())
        }
        onMcsifScmif:{
            codeInterface.setMCIf_SCMIf(value)
        }

        onScmifMcsif:{
            codeInterface.setSCMIf_MCIf(value)
        }

        onMifSwitch:{
            codeInterface.setMIf_Switch(value)
        }
        onSwitchMIf:{
            codeInterface.setSwitch_MIf(value)
        }
        onForWhile:{
            codeInterface.setFor_While(value)
        }
        onWhileFor:{
            codeInterface.setWhile_For(value)
        }
    }

    Connections{
        target: codeInterface
        onInputPathChanged:{
            textDisplayView.setLeftView(codeInterface.inputPath ,codeInterface.getInputFileData())
        }
		onParseError: {
            errorDialog.showError(e)
		}
	}

	Connections{
		target: textDisplayView
		onRefactorClicked:{
			var data=codeInterface.getRefactorData(textDisplayView.getLeftView())
			textDisplayView.setRightView(data)
		}
	}

    DropArea{
        anchors.fill: parent
        onDropped: {
            codeInterface.inputPath=drop.urls[0]
        }
    }
}
