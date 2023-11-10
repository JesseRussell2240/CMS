#include "backend.h"
#include <Windows.h>
#include "AudioRecorder.h"
#include <stdio.h>

extern short iBigBuf[];     // buffer
extern long lBigBufSize;    // total number of samples

// Constructor - initializes all variables
BackEnd::BackEnd(QObject *parent) : QObject(parent){
    // Initialize variables - STUDENTS: Add any variable you want to initialize here

    m_countRxTextQueue = 0;
    m_countRxAudioQueue = 0;
}

// STUDENTS: ADD YOUR CODE TO THE FUNCTIONS IN THIS SECTION BY CALLING FUNCTIONS YOU WRITE IN OTHER FILES (e.g. Message.h, RS232.h, Audio.h)
/************** These fuctions are known as Qt 'Slots' (C/C++ functions that respond to Button clicks from the QML Side) ****************/
/****************************************************************************************************************************************/
void BackEnd::transmitText() {
    // Call the RS232 Transmit function - need to include the header
    qDebug("C/C++ Transmit function called");

    // Testing the Text Queue counter indicator (temporary - to be moved to the receive function later and use the count() function for the text Queue)
    m_countRxTextQueue += 1;
    emit countRxTextQueueChanged();
}

void BackEnd::genQuote() {
    // Call the quote generation function - need to include the header
    qDebug("C/C++ Quote generation function called");
}

void BackEnd::recordAudio() {

  //  RecordAudioTB(iBigBuf, lBigBufSize, 5, 8 * 1000);
    // Call the record audio function - need to include the header
    qDebug("C/C++ record audio function called");
}

void BackEnd::playAudio() {
    // Call the play audio function - need to include the header
    qDebug("C/C++ play audio function called");


}

void BackEnd::saveAudio() {
    // Call the save audio function - need to include the header
    qDebug("C/C++ save audio function called");
}

void BackEnd::loadAudio() {
    // Call the load audio function - need to include the header
    qDebug("C/C++ load audio function called");
}

void BackEnd::transmitAudio() {
    // Call the transmit audio function - need to include the header
    qDebug("C/C++ transmit audio function called");

    // Testing the Audio Queue counter indicator (temporary - to be moved to the receive function later and use the count() function for the audio Queue)
    m_countRxAudioQueue += 1;
    emit countRxAudioQueueChanged();
}

void BackEnd::receiveText() {
    // Call the receive text function - need to include the header
    // Note that if the receive operation is multithreaded then can change the use for this button to 'dequeue' one item at a time instead
    qDebug("C/C++ receive text function called");
}

void BackEnd::viewText() {
    // Call the view text function - need to include the header
    qDebug("C/C++ view text function called");
}

void BackEnd::receiveAudio() {
    // Call the receive audio function - need to include the header
    // Note that if the receive operation is multithreaded then can change the use for this button to 'dequeue' one item at a time instead
    qDebug("C/C++ receive audio function called");
}

void BackEnd::playReceivedAudio() {
    // Call the play received audio function - need to include the header
    qDebug("C/C++ play received audio function called");
}



/**************************************************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************************/
/* DO NOT MODIFY ANYTHING IN THE SECTION BELOW, IT CONNECTS THE DROP DOWN SELECTIONS IN THE GUI (i.e., FRONT END QML VARIABLES) TO C/C++ VARIABLES ACCESIBLE TO THE BACK END!!!!
 * Q_PROPERTY functions #1-17 for connecting C/C++ properties (private variables) to QML variables on the front end ***/
/*******************************************************************************************************************************************************************************/

//#1 Transmitted Text
QString BackEnd::TxText() { return m_TxText; }  // Getter
void BackEnd::setTxText(const QString &txText) {
    if(txText == m_TxText) { return; }
    m_TxText = txText;
    emit TxTextChanged();
}

//#2 Received Text
QString BackEnd::RxText() { return m_RxText; }  // Getter
void BackEnd::setRxText(const QString &rxText) {
    if(rxText == m_RxText) { return; }
    m_RxText = rxText;
    emit RxTextChanged();
}

//#3 Priority
QString BackEnd::priority() { return m_priority; }  // Getter
void BackEnd::setPriority(const QString &pty) {     // Setter
    if(pty == m_priority) { return; }
    m_priority = pty;
    qDebug("Priority changed to: %s", m_priority.toLatin1().data() );
    emit priorityChanged();
}

//#4 Sample Rate - Need to convert this to an integer when using it
QString BackEnd::sampleRate() { return m_sampleRate; }  // Getter
void BackEnd::setSampleRate(const QString &sr) {     // Setter
    if(sr == m_sampleRate) { return; }
    m_sampleRate = sr;
    qDebug("Sample Rate changed to: %s", m_sampleRate.toLatin1().data() );
    qDebug("Convert QString to int, value is: %d", m_sampleRate.toInt() );
    emit sampleRateChanged();
}

//#5
int BackEnd::recTime() { return m_recTime; }         // Getter
void BackEnd::setRecTime(const int &rt) {            // Setter
    if(rt == m_recTime) { return; }
    m_recTime = rt;
    qDebug("Rec time changed to: %d", m_recTime );
    emit recTimeChanged();
}

//#6 Bit Rate - Need to convert this to an integer when using it
QString BackEnd::bitRate() { return m_bitRate; }     // Getter
void BackEnd::setBitRate(const QString &br) {        // Setter
    if(br == m_bitRate) { return; }
    m_bitRate = br;
    qDebug("Bit Rate changed to: %s", m_bitRate.toLatin1().data() );
    qDebug("Convert QString to int, value is: %d", m_bitRate.toInt() );
    emit bitRateChanged();
}

//#7 Com Port for current program
QString BackEnd::comPort() { return m_comPort; }     // Getter
void BackEnd::setComPort(const QString &cp) {        // Setter
    if(cp == m_comPort) { return; }
    m_comPort = cp;
    qDebug("COM Port changed to: %s", m_comPort.toLatin1().data() );
    emit comPortChanged();
}

//#8 Rx ID
QString BackEnd::rxID() { return m_rxID; }          // Getter
void BackEnd::setRxID(const QString &rxid) {        // Setter
    if(rxid == m_rxID ) { return; }
    m_rxID = rxid;
    qDebug("Rx ID changed to: %s", m_rxID.toLatin1().data() );
    emit rxIDChanged();
}

//#9 Tx ID
QString BackEnd::txID() { return m_txID; }          // Getter
void BackEnd::setTxID(const QString &txid) {        // Setter
    if(txid == m_txID ) { return; }
    m_txID = txid;
    qDebug("Tx ID changed to: %s", m_txID.toLatin1().data() );
    emit txIDChanged();
}

//#10
QString BackEnd::enc() { return m_enc; }          // Getter
void BackEnd::setenc(const QString &en) {        // Setter
    if(en == m_enc ) { return; }
    m_enc = en;
    qDebug("Encryption set to: %s", m_enc.toLatin1().data() );
    emit encChanged();
}

//#11
QString BackEnd::cmp() { return m_cmp; }          // Getter
void BackEnd::setcmp(const QString &c) {        // Setter
    if(c == m_cmp ) { return; }
    m_cmp = c;
    qDebug("Compression set to: %s", m_cmp.toLatin1().data() );
    emit cmpChanged();
}

//#12
QString BackEnd::timeout() { return m_timeout; }     // Getter
void BackEnd::setTimeout(const QString &to) {        // Setter
    if(to == m_timeout ) { return; }
    m_timeout = to;
    qDebug("Timeout set to: %s", m_timeout.toLatin1().data() );
    qDebug("Convert QString to int, value of timeout is: %d", m_timeout.toInt() );
    emit timeoutChanged();
}

//#13
QString BackEnd::options() { return m_options; }     // Getter
void BackEnd::setOptions(const QString &opt) {        // Setter
    if(opt == m_options ) { return; }
    m_options = opt;
    qDebug("Options set to: %s", m_options.toLatin1().data() );
    //qDebug("Convert QString to int, value of options is: %d", m_options.toInt() );
    emit optionsChanged();
}

//#14
bool BackEnd::pty() { return m_pty; }               // Getter
void BackEnd::setPty(const bool &tf) {               // Setter
    if(tf == m_pty ) { return; }
    m_pty = tf;
    qDebug("Priority Queue set to: %d", m_pty );
    emit ptyChanged();
}

//#15
QString BackEnd::rxAudText() { return m_rxAudText; }     // Getter
void BackEnd::setRxAudText(const QString &rxat) {        // Setter
    if(rxat == m_rxAudText ) { return; }
    m_rxAudText = rxat;
    qDebug("Received headers of audio file: %s", m_rxAudText.toLatin1().data() );
    emit rxAudTextChanged();
}

//#16
int BackEnd::countRxTextQueue() { return m_countRxTextQueue; }  // Getter
void BackEnd::setCountRxTextQueue(const int &ctt) {             // Setter
    if(ctt == m_countRxTextQueue) { return; }
    m_countRxTextQueue = ctt;
    qDebug("Text Message Queue Count: %d", m_countRxTextQueue);
    emit countRxTextQueueChanged();
}

//#17
int BackEnd::countRxAudioQueue() { return m_countRxAudioQueue; }  // Getter
void BackEnd::setCountRxAudioQueue(const int &cta) {             // Setter
    if(cta == m_countRxAudioQueue) { return; }
    m_countRxAudioQueue = cta;
    qDebug("Audio Message Queue Count: %d", m_countRxAudioQueue);
    emit countRxAudioQueueChanged();
}

