/*
 * @date: 13/06/18
 * @author: (original author) Harry J.E Day <harry@dayfamilyweb.com>
 * @authors: (editors) 
 * @details: Header for can functionality
 * @copydetails: This code is released under the LGPLv3 and newer License and the BSD License
 * @copyright: Copyright BLUEsat UNSW 2017
 */

#include "boilerplate.h"
#include "rtos-kochab.h"

#ifndef PROJECT_CAN_H
#define PROJECT_CAN_H

namespace ros_echronos {
/**
 * Namespace for can specific functionality
 */
    namespace can {
        /**
         * the number of bits to skip when making custom control headers
         */
        constexpr uint8_t HEADER_COMMON_BITS = 12;
        extern int can_error_flag;

        /**
         * The maximum length of a can message
         */
        constexpr uint8_t CAN_MESSAGE_MAX_LEN = 8;

        /**
         * Value for the ros mode on the can bus
         */
        constexpr uint8_t ROS_CAN_MODE = 1;
        /**
         * Indicates that the "seq_num" field of a can message header is in special mode
         */
        constexpr uint8_t SEQ_NUM_SPECIAL_MODE = 7;


        /**
         * Possible function modes on the can buss
         */
        typedef enum _ROS_Function {
            /**
             * Means the can message is part of a message transmission
             */
                FN_ROS_MESSAGE_TRANSMISSION = 0,
            /**
             * Means the can message is part of a service call (Not implemented)
             */
                FN_ROS_SERVICE = 1,
            /**
             * Means the can message is a control message
             */
                FN_ROS_CONTROL_MSG = 2,
            /**
             * Undefined functionality, reserved for future expansion of the protocol
             */
                FN_ROS_RESERVED = 3
        } ROS_Function;

        /**
         * Possible Control Functions
         */
         typedef enum _ctrl_function {
                REGISTER_NODE = 0,
                DEREGISTER_NODE = 1,
                SUBSCRIBE_TOPIC = 2,
                UNSUBSCRIBE_TOPIC = 3,
                ADVERTISE_TOPIC = 4,
                DEREGISTER_ADVERTISED_TOPIC = 5,
                ADVERTISE_SERVICE = 6,
                DEREGISTER_SERIVCE = 7,
                MANAGE_PARAMETERS = 8,
                CHANNEL_CONTROL = 9,
                EXTENDED = 10
         } Ctrl_Function;
	 
	 constexpr unsigned int BASE_FIELD_SIZE = 8;

        /**
         * Represents a can header as per the ros over can protocol
         */
        typedef union can_header {
            unsigned long bits;
            union _header_fields {
                // these MUST be ints to make the compiler place them in the same 32-bit container
                // See: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0491i/Babjddhe.html
                // (Also gcc docs, but they are less clear)
                struct _base_fields {
                    unsigned int mode : 1;
                    unsigned int priority : 2;
                    unsigned int ros_function : 2;
                    unsigned int seq_num : 3;
                    unsigned int mode_specific : 28 - BASE_FIELD_SIZE;
                } __attribute__((packed)) base_fields;
                /**
                 * Header fields for ROS msgs
                 */
                struct _ros_msg_fields {
                    unsigned int base_specific : BASE_FIELD_SIZE;
                    unsigned int message_num : 2;
                    unsigned int topic : 7;
                    unsigned int message_length : 8;
                    unsigned int node_id : 4;
                    unsigned int not_in_range : 3;
                } __attribute__((packed)) f0_ros_msg_fields;
                /**
                 * Header fields for control msgs
                 */
                struct _ros_ctrl_fields {
                    unsigned int empty: BASE_FIELD_SIZE;
                    unsigned int mode : 4;
                    unsigned int control_specific : 28 - BASE_FIELD_SIZE - 4;
                } __attribute__((packed)) f2_ctrl_msg_fields;
            } __attribute__((packed)) fields;
        } CAN_Header;

        /**
         * Represents a can packet.
         */
        typedef struct can_ros_message {
            CAN_Header head;
            uint8_t body[CAN_MESSAGE_MAX_LEN];
            uint8_t body_bytes;
        } CAN_ROS_Message __attribute__((aligned(4)));

        /**
         * Standard header base for ctrl messages
         */
        constexpr CAN_Header CAN_CTRL_BASE_FIELDS = {
            .fields = {
                .base_fields = {
                    ((unsigned int) ROS_CAN_MODE),
                    0,
                    ((unsigned int) FN_ROS_CONTROL_MSG),
                    0,
                    0
                }
            }
        };

        /**
         * Mask for the subscriber ctrl header base fields
         */
        constexpr CAN_Header _CTRL_HEADER_MASK_BASE_FIELDS {
            .fields = {
                .base_fields = {
                    1, 0x0000, 0xFFFF, 0xFFF, 0x0
                }
            }
        };

        constexpr CAN_Header _CTRL_HEADER_MASK_F2_FIELDS {
            .fields = {
                .f2_ctrl_msg_fields = {
                    0x0, 0xFFF, 0xFFF
                }
            }
        };


        /**
         * We can't use mutexes in buffers so we have to assume that we do not receive another message
         * before this is written. I eventually want to replace this with a lockless queue.
         */
        typedef struct _input_buffer {
            CAN_ROS_Message buffer;
            int start_counter = 0;
            int end_counter = 0;
        } input_buffer_t;

        constexpr CAN_Header _TOPIC_BITMASK_BASE = {
            .fields = {
                .base_fields = { 1, 0, 0xF, 0},
            }
        };
        constexpr CAN_Header _TOPIC_BITMASK_F0 = {
            .fields = {
                .f0_ros_msg_fields = { 0x0, 0, 0xFFFF, 0, 0, 0}
            }
        };


        inline const unsigned long getTopicHeaderBitmask() {
            return _TOPIC_BITMASK_BASE.bits | _TOPIC_BITMASK_F0.bits;
        }


        extern RtosInterruptEventId can_interupt_event;
        extern volatile bool node_handle_ready;

        /**
         * Hashes a null terminated string
         */
        uint8_t hash(const char * name);
    }
}
#endif //PROJECT_CAN_H
