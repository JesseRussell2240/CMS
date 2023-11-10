#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

// NOTE TO STUDENTS: You should not need to edit this file, except maybe to add private variables (see below)

class BackEnd : public QObject
{
    Q_OBJECT
    // Set up connection between QML and C++ variables (first parameter in Q_PROPERTY macro)
    // Using the Q_PROPERTY Macro you can use the variable in QML directly as you would use in C++
    // C++ triggers changes in QML (front end GUI) by emitting signals (see in the 'set' functions) below to change the variable in QML
    Q_PROPERTY(QString TxText READ TxText WRITE setTxText NOTIFY TxTextChanged)            // #1
    Q_PROPERTY(QString RxText READ RxText WRITE setRxText NOTIFY RxTextChanged)            // #2
    Q_PROPERTY(QString priority READ priority WRITE setPriority NOTIFY priorityChanged)    // #3
    Q_PROPERTY(QString sampleRate READ sampleRate WRITE setSampleRate NOTIFY sampleRateChanged) // #4
    Q_PROPERTY(int recTime READ recTime WRITE setRecTime NOTIFY recTimeChanged)            // #5
    Q_PROPERTY(QString bitRate READ bitRate WRITE setBitRate NOTIFY bitRateChanged)        // #6
    Q_PROPERTY(QString comPort READ comPort WRITE setComPort NOTIFY comPortChanged)        // #7
    Q_PROPERTY(QString rxID READ rxID WRITE setRxID NOTIFY rxIDChanged)                    // #8
    Q_PROPERTY(QString txID READ txID WRITE setTxID NOTIFY txIDChanged)                    // #9
    Q_PROPERTY(QString enc READ enc WRITE setenc NOTIFY encChanged)                        // #10
    Q_PROPERTY(QString cmp READ cmp WRITE setcmp NOTIFY cmpChanged)                        // #11
    Q_PROPERTY(QString timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)        // #12
    Q_PROPERTY(QString options READ options WRITE setOptions NOTIFY optionsChanged)        // #13
    Q_PROPERTY(bool pty READ pty WRITE setPty NOTIFY ptyChanged)                           // #14
    Q_PROPERTY(QString rxAudText READ rxAudText WRITE setRxAudText NOTIFY rxAudTextChanged) // #15
    Q_PROPERTY(int countRxTextQueue READ countRxTextQueue WRITE setCountRxTextQueue NOTIFY countRxTextQueueChanged) // #16
    Q_PROPERTY(int countRxAudioQueue READ countRxAudioQueue WRITE setCountRxAudioQueue NOTIFY countRxAudioQueueChanged) //#17

public:
    explicit BackEnd(QObject *parent = nullptr);

    //#1
    QString TxText();                       // Getter
    void setTxText(const QString &txText);  // Setter
    //#2
    QString RxText();                       // Getter
    void setRxText(const QString &rxText);  // Setter
    //#3
    QString priority();                     // Getter
    void setPriority(const QString &pty);   // Setter
    //#4
    QString sampleRate();                   // Getter
    void setSampleRate(const QString &sr);  // Setter
    //#5
    int recTime();                          // Getter
    void setRecTime(const int &rt);         // Setter
    //#6
    QString bitRate();                      // Getter
    void setBitRate(const QString &br);     // Setter
    //#7
    QString comPort();                      // Getter
    void setComPort(const QString &cp);     // Setter
    //#8
    QString rxID();                         // Getter
    void setRxID(const QString &rxid);      // Setter
    //#9
    QString txID();                         // Getter
    void setTxID(const QString &txid);      // Setter
    //#10
    QString enc();                          // Getter
    void setenc(const QString &en);         // Setter
    //#11
    QString cmp();                          // Getter
    void setcmp(const QString &c);          // Setter
    //#12
    QString timeout();                      // Getter
    void setTimeout(const QString &to);     // Setter
    //#13
    QString options();                      // Getter
    void setOptions(const QString &opt);    // Setter
    //#14
    bool pty();                            // Getter
    void setPty(const bool &tf);           // Setter
    //#15
    QString rxAudText();                     // Getter
    void setRxAudText(const QString &rxat);  // Setter
    //#16
    int countRxTextQueue();                 // Getter
    void setCountRxTextQueue(const int &ctt);             // Setter
    //#17
    int countRxAudioQueue();                 // Getter
    void setCountRxAudioQueue(const int &cta);             // Setter


public slots:                   // Slots are C/C++ functions called by Signals from QML (e.g. button clicks)
    void transmitText();        // call the transmit text function
    void genQuote();            // Generate new quote from file and set the value of the transmitted text to the new quote
    void recordAudio();         // Record audio
    void playAudio();           // Play audio recording
    void saveAudio();           // Save audio
    void loadAudio();           // load audio file
    void transmitAudio();       // Transmit audio file and header
    void receiveText();         // Receive text button pushed
    void viewText();            // View text button pushed
    void receiveAudio();        // Receive the audio and header text (set rxAudioText to the header values)
    void playReceivedAudio();   // Play the received audio frin the buffer

signals:                        // Signals come from C++ (via emit) and QML (via 'var = x' statements) to call the Q_PROPERTY functions to update the QML GUI and call C/C++ functions
    void TxTextChanged();       //#1
    void RxTextChanged();       //#2
    void priorityChanged();     //#3
    void sampleRateChanged();   //#4
    void recTimeChanged();      //#5
    void bitRateChanged();      //#6
    void comPortChanged();      //#7
    void rxIDChanged();         //#8
    void txIDChanged();         //#9
    void encChanged();          //#10
    void cmpChanged();          //#11
    void timeoutChanged();      //#12
    void optionsChanged();      //#13
    void ptyChanged();          //#14
    void rxAudTextChanged();    //#15
    void countRxTextQueueChanged();  //#16
    void countRxAudioQueueChanged(); //#17

private:
    QString m_TxText;       //#1
    QString m_RxText;       //#2
    QString m_priority;     //#3
    QString m_sampleRate;   //#4
    int m_recTime;          //#5
    QString m_bitRate;      //#6
    QString m_comPort;      //#7
    QString m_rxID;         //#8
    QString m_txID;         //#9
    QString m_enc;          //#10
    QString m_cmp;          //#11
    QString m_timeout;      //#12
    QString m_options;      //#13
    bool m_pty;             //#14 fifo vs priority order
    QString m_rxAudText;    //#15 Rx audio header text
    int m_countRxTextQueue; //#16 count of received text messages in the text message queue
    int m_countRxAudioQueue;//#17 count of received audio messages in the audio message queue (if desired)

    // Other private variables, as required
    // STUDENTS: You can add private variables here if needed


};

#endif // BACKEND_H

