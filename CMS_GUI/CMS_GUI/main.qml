 /* main.qml : Contains the GUI front end components - This file has been edited to work with Qt v6 components
  * Date: Oct 2021
  * Author: Michael A. Galle
  */

// Order of these imports matters
//import QtQuick.Controls 1.4   // Removed in Qt v6
import QtQuick.Controls 2.5     // 2.5
import QtQuick.Window 2.12
import QtQml.Models 2.2
import QtQuick 2.4
import Qt.labs.platform 1.1     // was QtQuick.Dialogs 1.2  but changed in Qt v6
import QtQuick.Layouts 1.3
import QtQuick 2.13             // was 2.12

// Registered C ++ classes - This is how we import classes registered with QML from C++ (see registration in main.cpp)
import io.qt.backend 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 850
    id: myWindow
    title: qsTr("Coded Messaging System")

    // Imported classes as QML components (functions and variables) that are written in C/C++
    BackEnd {
        id: backend       // Imported class for communication with C/C++ side
    }

    /* ********************* sub menu actions/functions *********************************************** */

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
    }

    /* ********************* Menus *********************************************** */
    MenuBar {
        Menu {                                      // File
            title: qsTr("&File")
            font: font.pointSize = 10
            MenuItem {
                text: qsTr("&New")
                onTriggered: actionNew              // Add the New function
            }
            MenuItem {
                text: qsTr("&Open")
                onTriggered: fileDialog.open()      // Open functionality added here
            }
            MenuItem {
                text: qsTr("&Save")
                onTriggered: actionSave             // Add the Save function here
            }
            MenuItem {
                text: qsTr("&Exit")
                onTriggered: myWindow.close()       // Add the Exit/Quit function here
            }
        }
        Menu {                                      // View
            title: qsTr("&View")
            font: font.pointSize = 10
            MenuItem {
                text: qsTr("&Zoom in")
                onTriggered : actionZoomIn
            }
            MenuItem {
                text: qsTr("&Zoom out")
                onTriggered : actionZoomOut
            }
        }
        Menu {                                      // Help
            title: qsTr("&Help")
            font: font.pointSize = 10
            MenuItem {
                text: qsTr("&About")
                onTriggered: actionAbout
            }
            MenuItem {
                text: qsTr("&Config")
                onTriggered: actionConfig
            }
        }
    }

     /* ********************* UI Form (MAIN CONTROLS) *********************************************** */

    Item {
        id: txForm
        width: 1200
        height: 800
        property alias textInput: textInput

        Page {
            id: page
            x: 0
            y: 0
            width: 1200
            height: 800
            anchors.left: parent.left
            anchors.topMargin: 0
            anchors.top: parent.top
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            /*********** Left side of Window (Transmit Text and Audio) *************/
            // Bounding box
            GroupBox {
                id: txGroupBox
                y: 30
                width: 525
                height: 538
                font.bold: true
                anchors.left: parent.left
                anchors.leftMargin: 50
                topPadding: 35
                font.pointSize: 10
                title: qsTr("Transmit")

                GroupBox {
                    id: textGroupBox
                    x: 0
                    y: 5
                    width: 500
                    height: 225
                    topPadding: 35
                    leftPadding: 12
                    font.pointSize: 10
                    title: qsTr("Text")

                    ScrollView {
                        id: textScrollView
                        width: 410
                        height: 170
                        anchors.top: parent.top
                        anchors.left: parent.left

                        TextArea {
                            id: textInput
                            x: 0
                            y: 0
                            width: 400
                            height: 170
                            wrapMode: TextArea.Wrap
                            text: backend.TxText
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 0
                            anchors.topMargin: 0
                            anchors.top: parent.top
                            anchors.left: parent.left
                            readOnly: false
                            font.pointSize: 8
                            selectionColor: "#010080"
                            cursorVisible: true
                            onTextChanged: backend.TxText = text
                        }
                    }

                    Button {
                        id: transmitText
                        x: 416
                        y: 49
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Send")
                        font.pointSize: 8
                        onClicked: {
                            console.log("Transmit text button indicator in console (JavaScript)")
                            backend.RxText = backend.TxText  // Simulating TxRx operation - comment this out later when using the Receive functions
                            backend.transmitText()
                        }
                    }

                    Button {
                        id: genQuotes
                        x: 416
                        y: 92
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Gen")
                        font.pointSize: 8
                        anchors.bottom: transmitText.top
                        anchors.right: parent.right
                        onClicked: {
                            console.log("Generate random quotes button indicator in console (JavaScript)")
                            // backend.TxText = backend.generate()    // need to make a slot called generate() that calls the random quote generator
                            backend.genQuote()
                        }
                    }
                }

                // Audio Record, Play, Load controls
                GroupBox {
                    id: audioGroup
                    title: qsTr("Audio")
                    x: 20
                    y: 250
                    width: 438
                    height: 240
                    anchors.right: textGroupBox.right
                    anchors.left: textGroupBox.left
                    padding: 12
                    bottomPadding: 12
                    topPadding: 35
                    font.wordSpacing: 0
                    spacing: 6
                    font.pointSize: 10

                    ComboBox {
                        id: comboBox
                        x: 186
                        y: 10
                        width: 165
                        height: 40
                        font.pointSize: 10
                        Label {
                            id: sampleRateLabel
                            x: -161
                            y: 13
                            text: qsTr("Sample Rate")
                            font.pointSize: 12
                        }

                        Label {
                            id: sampleRatekHz
                            x: 185
                            y: 13
                            text: qsTr("kHz")
                            font.pointSize: 10
                        }

                        currentIndex: 2        // Can get this value from a saved file to use a previoulsy saved setting
                        model: ["4", "6", "8", "10", "12", "14", "16"]
                        onActivated: {
                            backend.sampleRate = currentText
                        }

                    }

                    Slider {
                        id: recTimeSeconds
                        x: 195
                        y: 67
                        width: 235
                        height: 59
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 0
                        anchors.left: comboBox.left
                        stepSize: 1
                        font.pointSize: 18
                        live: true
                        value: 6
                        to: 20
                        snapMode: Slider.SnapAlways

                        Label {
                            id: recTimeLabel
                            x: -161
                            y: 21
                            width: 165
                            height: 33
                            text: qsTr("Record time (s)  0")
                            font.pointSize: 10
                        }

                        Label {
                            id: audioRecLabel
                            x: 237
                            y: 21
                            text: qsTr("20")
                            anchors.top: recTimeLabel.top
                            font.pointSize: 10
                        }

                        // change value when the slider is dragged
                        onMoved: {
                            backend.recTime = value;
                        }
                    }

                    Button {
                        id: recBtn
                        x: 48
                        y: 165
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Rec")
                        anchors.verticalCenterOffset: 1
                        anchors.topMargin: 1
                        anchors.bottom: playBtn.bottom
                        anchors.top: playBtn.top
                        font.pointSize: 8
                        onClicked: {
                            console.log("Record indicator in console (JavaScript)")
                            backend.recordAudio()
                        }
                    }

                    Button {
                        id: playBtn
                        x: 128
                        y: 144
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Play")
                        font.pointSize: 8
                        onClicked: {
                            console.log("Play indicator in console (JavaScript)")
                            backend.playAudio()
                        }
                    }

                    Button {
                        id: saveBtn
                        x: 208
                        y: 144
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Save")
                        font.pointSize: 8
                        onClicked: {
                            console.log("Save indicator in console (JavaScript)")


                            backend.saveAudio()
                        }
                    }

                    Button {
                        id: loadAudioBtn
                        x: 288
                        y: 144
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Load")
                        font.pointSize: 8
                        onClicked: {
                            console.log("Load indicator in console (JavaScript)")
                            backend.loadAudio()
                        }
                    }

                    Button {
                        id: sendAudio
                        x: 368
                        y: 144
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Send")
                        font.pointSize: 8
                        onClicked: {
                            console.log("Send indicator in console (JavaScript)")
                            // Simulate sending audio to receiver side
                            backend.rxAudText = "Sample Audio file header text to be generated by the RS232 receive function. Replace this with function that returns header text"
                            backend.transmitAudio()
                        }
                    }

                }
            }

            /*********** Right side of Window (Receive Text and Audio) *************/
            // Bounding box
            GroupBox {
                id: rxGroupBox
                x: 566
                y: 30
                width: 525
                height: 538
                font.bold: true
                anchors.right: parent.right
                anchors.rightMargin: 50
                topPadding: 35
                anchors.bottomMargin: 0
                anchors.verticalCenter: txGroupBox.verticalCenter
                anchors.bottom: txGroupBox.bottom
                font.pointSize: 10
                title: qsTr("Receive")

                // Receive Text
                GroupBox {
                    id: textGroupBox1
                    font.pointSize: 10
                    title: qsTr("Text")
                    x: 0
                    y: 5
                    width: 500
                    height: 225
                    topPadding: 35
                    anchors.right: audioGroupBox.right
                    anchors.left: parent.left


                    ScrollView {
                        id: textScrollViewRx
                        width: 410
                        height: 170
                        anchors.top: parent.top
                        anchors.left: parent.left
                        font.pointSize: 8

                        // Received Text Output
                        TextArea {
                            id: rxTextbox
                            x: 0
                            y: 0
                            width: 400
                            height: 170
                            wrapMode: TextArea.Wrap
                            anchors.rightMargin: 6
                            anchors.bottom: parent.bottom
                            anchors.top: parent.top
                            text: backend.RxText
                        }
                    }
                    // Receive Button for text - May be used to dequeue messages received in queue if the receive operation is multithreaded and this button is not required
                    Button {
                        id: rxBtn2
                        x: 416
                        y: 81
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Rx")
                        anchors.horizontalCenterOffset: 0
                        anchors.top: viewBtn.bottom
                        anchors.leftMargin: 0
                        anchors.topMargin: 6
                        font.pointSize: 8
                        onClicked: {
                            console.log("Receive text messages indicator in console (JavaScript)")
                            backend.receiveText()
                        }
                    }

                    // View Button for text
                    Button {
                        id: viewBtn
                        x: 416
                        y: 35
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("View")
                        anchors.top: txtMsgCount.bottom
                        anchors.topMargin: 18
                        font.pointSize: 8
                        onClicked: {
                            console.log("View button indicator in console (JavaScript)")
                            backend.viewText()
                        }
                    }

                    // Count of messages in Queue
                    Label {
                        id: txtMsgCount
                        x: 331
                        y: -35
                        text: qsTr("Count: " + backend.countRxTextQueue.toString())
                        anchors.top: parent.top
                        anchors.topMargin: -35
                        font.pointSize: 10
                    }
                }

                // Receive Audio
                GroupBox {
                    id: audioGroupBox
                    title: qsTr("Audio")
                    x: 0
                    y: 250
                    width: 500
                    height: 240
                    topPadding: 35
                    anchors.left: parent.left
                    font.pointSize: 10

                    // Receive Audio button - May be used to dequeue messages received in queue if the receive operation is multithreaded and this button is not required
                    Button {
                        id: rxBtn1
                        x: 416
                        y: 70
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Rx")
                        anchors.horizontalCenterOffset: 0
                        anchors.leftMargin: 0
                        anchors.horizontalCenter: viewBtn1.horizontalCenter
                        anchors.top: viewBtn1.bottom
                        anchors.topMargin: 6
                        anchors.left: viewBtn1.left
                        font.pointSize: 8
                        onClicked: {
                            console.log("Rx (audio) button indicator in console (JavaScript)")
                            backend.receiveAudio()
                        }
                    }

                    Button {
                        id: viewBtn1
                        x: 416
                        y: -7
                        width: 60
                        height: 40
                        leftPadding: 0
                        rightPadding: 0
                        text: qsTr("Play")
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        font.pointSize: 8
                        onClicked: {
                            console.log("Play button (received audio) indicator in console (JavaScript)")
                            backend.playReceivedAudio()
                        }
                    }
                    ScrollView {
                        id: textAudioScrollViewRx
                        width: 410
                        height: 190
                        anchors.top: parent.top
                        anchors.left: parent.left
                        font.pointSize: 8

                        TextArea {
                            id: rxAudTextHeaders
                            x: 0
                            y: 0
                            width: 410
                            height: 190
                            wrapMode: TextArea.Wrap
                            anchors.rightMargin: 6
                            anchors.bottom: parent.bottom
                            anchors.top: parent.top
                            text: backend.rxAudText            // Added to show headers of RxAudio message
                        }
                    }

                    Label {
                        id: audioMsgCount
                        x: 336
                        y: -35
                        width: 150
                        height: 22
                        text: qsTr("Count: " + backend.countRxAudioQueue.toString())
                        anchors.top: parent.top
                        anchors.topMargin: -35
                        font.pointSize: 10
                    }

                }

            }

            /*********** Bottom Full Width of Window *************/
            // Bounding box
            GroupBox {
                id: commSettings
                y: 600
                width: 1100
                height: 143
                font.bold: true
                anchors.left: parent.left
                anchors.leftMargin: 50
                leftPadding: 12
                topPadding: 35
                wheelEnabled: true
                spacing: 8
                font.pointSize: 10
                title: qsTr("Communication Settings")

                // Layout dropdown items in a grid
                GridLayout {
                    id: gridLayout
                    x: 0
                    y: 29
                    width: 1085
                    height: 94
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    rows: 1
                    columns: 8

                    // Bit Rate
                    ComboBox {
                        id: comboBox1
                        Label {
                            id: brLabel
                            y: -20
                            width: 100
                            height: 20
                            text: qsTr("Bit rate")
                        }
                        currentIndex: 0        // Can get this value from a saved file to use a previoulsy saved setting
                        model: ["9600", "19200", "28800", "38400", "48000", "57600"]
                        onActivated: {
                            backend.bitRate = currentText
                        }
                    }

                    // COM Port
                    ComboBox {
                        id: comboBox2
                        Label {
                            id: comLabel
                            y: -20
                            width: 100
                            height: 20
                            text: qsTr("COM/ttyS port")
                        }
                        currentIndex: 0        // Can get this value from a saved file to use a previoulsy saved setting
                        model: ["COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "ttyUSB0", "ttyUSB1", "ttyUSB2"]
                        onActivated: {
                            backend.comPort = currentText;
                        }
                    }

                    // Receiver ID
                    ComboBox {
                        id: comboBox3
                        Label {
                            id: rxIDLabel
                            y: -20
                            width: 100
                            height: 20
                            text: qsTr("Rx ID")
                        }
                        currentIndex: 0        // Can get this value from a saved file to use a previoulsy saved setting
                        model: ["0x100", "0x101", "0x102", "0x201", "0x202", "0x203"]
                        onActivated: {
                            backend.rxID = currentText
                        }

                    }

                    // Sender ID
                    ComboBox {
                        id: comboBox4
                        Label {
                            id: txIDLabel
                            y: -20
                            width: 100
                            height: 20
                            text: qsTr("Tx ID")
                        }
                        currentIndex: 0        // Can get this value from a saved file to use a previoulsy saved setting
                        model: ["0x100", "0x101", "0x102", "0x201", "0x202", "0x203"]
                        onActivated: {
                            backend.txID = currentText
                        }
                    }

                    // Encryption
                    ComboBox {
                        id: comboBox5
                        Label {
                            id: encLabel
                            y: -20
                            width: 100
                            height: 20
                            text: qsTr("Encryption")
                        }
                        currentIndex: 0        // Can get this value from a saved file to use a previoulsy saved setting
                        model: ["None", "XOR", "Vigenere"]
                        onActivated: {
                            backend.enc = currentText;
                        }
                    }

                    // Compression
                    ComboBox {
                        id: comboBox6
                        Label {
                            id: compLabel
                            y: -20
                            width: 100
                            height: 20
                            text: qsTr("Compression")
                        }
                        currentIndex: 0        // Can get this value from a saved file to use a previoulsy saved setting
                        model: ["None", "RLE", "Huffman", "Both"]
                        onActivated: {
                            backend.cmp = currentText;
                        }
                    }

                    // Timeout
                    ComboBox {
                        id: comboBox7
                        Label {
                            id: waitLabel
                            y: -20
                            width: 100
                            height: 20
                            text: qsTr("Timeout (s)")
                        }
                        currentIndex: 0        // Can get this value from a saved file to use a previoulsy saved setting
                        model: ["2", "10", "15", "20"]
                        onActivated: {
                            backend.timeout = currentText
                        }
                    }

                    ComboBox {
                        id: comboBox8
                        Label {
                            id: modeLabel
                            y: -20
                            width: 100
                            height: 20
                            text: qsTr("Options")
                        }

                        currentIndex: 0        // Can get this value from a saved file to use a previously saved setting
                        model: ["opt1", "opt2", "opt3", "opt4"]
                        onActivated: {
                            backend.options = currentText
                        }
                    }


                }
            }

            /*********** Priority Buttons ************************/
            Switch {
                id: element
                checked: false
                x: 1035
                y: 565
                text: qsTr("Priority")
                font.pointSize: 10
                onToggled: {
                    backend.pty = checked
                }
            }

            Label {
                id: label
                width: 56
                height: 15
                text: qsTr("FIFO")
                font.pointSize: 10
                anchors.rightMargin: -18
                anchors.right: element.left
                anchors.verticalCenter: element.verticalCenter
                anchors.verticalCenterOffset: 0
            }

            ComboBox {
                id: priority
                width: 93
                height: 37
                anchors.rightMargin: 50
                anchors.right: label.left
                anchors.verticalCenter: label.verticalCenter
                anchors.verticalCenterOffset: 5

                Label {
                    id: priorityLabel
                    y: 10
                    x: -70
                    font.pointSize: 10
                    text: qsTr("Priority")
                }

                currentIndex: 0        // Can get this value from a saved file to use a previoulsy saved setting
                model: ["1", "2", "3", "4", "5", "6", "7"]
                onActivated: {
                    backend.priority = currentText;
                }
            }

        }
    }
}

