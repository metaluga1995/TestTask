import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: 640
    height: 320
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    title: qsTr("TestTask")

    signal sendParamethers(string startURL, int cores, int linksNumber, string subString)
    Button {
        id: start
        x: 350
        y: 10
        width: 60
        height: 40
        text: qsTr("Start")
        enabled : true
        onClicked:
        {
            if (urlField.text == "" || coreNumber.text < 1 || linksNumber.text <1 || subStr.text == "")
            {
                log.text += "Please fill all paramethers\n"
            }
            else
            {
                progress.value = 0
                enabled = false
                pause.enabled = true
                pause.text = "Pause"
                stop.enabled = true
                urlField.readOnly = true
                coreNumber.readOnly = true
                linksNumber.readOnly = true
                subStr.readOnly = true
                sendParamethers(urlField.text, coreNumber.text, linksNumber.text, subStr.text)
            }
        }

    }
    signal sendPauseCommand(bool paused)
    Button {
        id: pause
        x: 450
        y: 10
        width: 60
        height: 40
        text: qsTr("Pause")
        enabled: false
        onClicked:
        {

            if (text == "Pause")
            {
                sendPauseCommand(true)
                text = "Resume"
            }
            else
            {
                sendPauseCommand(false)
                text = "Pause"

            }

        }
    }
    function stopButton(){
        start.enabled = true
        pause.enabled = false
        stop.enabled = false
        pause.text = "Pause"
        progress.value = 0
        urlField.readOnly = false
        coreNumber.readOnly = false
        linksNumber.readOnly = false
        subStr.readOnly = false
    }

    signal sendStopCommand()
    Button {
        id: stop
        x: 550
        y: 10
        width: 60
        height: 40
        text: qsTr("Stop")
        enabled: false
        onClicked:
        {
            sendStopCommand()
            stopButton()
        }
    }

    TextField {
        id: urlField
        x: 10
        width: 300
        placeholderText: qsTr("Start URL")

    }
    TextField {
        id: coreNumber
        x: 10
        width: 300
        y: 30
        validator: IntValidator {bottom: 1; top: 40}
        placeholderText: qsTr("Core number")

    }

    TextField {
        id: linksNumber
        x: 10
        width: 300
        y: 60
        validator: IntValidator {bottom: 1}
        placeholderText: qsTr("Max. parse links")

    }

    TextField {
        id: subStr
        x: 10
        width: 300
        y: 90
        placeholderText: qsTr("Searchable Text")

    }

    function updateProgress(value){
        progress.value = value
    }

    ProgressBar {
        id: progress
        value: 0
        width: 610
        x: 10
        y: 290
    }

    function updateLog(logString){
        log.text += logString + "\n"
    }

    TextArea {
        id: log
        x: 10
        y: 130

        width: 300
        text: ""
        onTextChanged: { log.cursorPosition = log.length }
    }

    function updateOutput(outpuString){
        output.text += outpuString + "\n"
    }
    TextArea {
        id: output
        x: 320
        y: 130

        width: 300
        text: ""
        onTextChanged: { output.cursorPosition = output.length }
    }
}
