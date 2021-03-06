#ifndef MESSAGE_H
#define MESSAGE_H

#define NUM_MSG 8

enum messages {
    FLD,
    FRD,
    BLD,
    BRD,
    FLS,
    FRS,
    BLS,
    BRS
};

struct message {
    double data[NUM_MSG];
    uint32_t endMagic;
} __attribute__((packed)) ; 

struct messageAdapter {
    union _data {
        struct message msg;
        uint8_t structBytes[sizeof(struct message)];
    } data;
};

//const unsigned char startMagic[4] = {0xFE, 0xED, 0xBE, 0xEF};
const unsigned char startMagic[4] = {0xEF, 0xBE, 0xED, 0xFE};
const uint32_t endMagic = 0xDEADBEEF;

#endif
