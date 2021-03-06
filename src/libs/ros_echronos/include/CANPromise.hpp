/*
 * @date: 17/06/18
 * @author: (original author) Harry J.E Day <harry@dayfamilyweb.com>
 * @authors: (editors)
 * @details: Simple Promise Library for CAN Messages
 * @copydetails: This code is released under the LGPLv3 and newer License and the BSD License
 * @copyright: Copyright BLUEsat UNSW 2017
 */

#include "include/can.hpp"
#include "rtos-kochab.h"

#ifndef PROJECT_CANPROMISE_H
#define PROJECT_CANPROMISE_H

namespace ros_echronos {
    namespace promise {
        class CANPromise;
        /**
         * Class that manages CAN Promises
         * @tparam N the size of the promise queue
         */
        class CANPromiseManager {
            public:
                /**
                 * Creates a new promise manager
                 * @param buffer the buffer
                 * @param bitmask a bitmask with at least as many bits as the size of the buffer
                 * @param buffer_size the size of the buffer in CANPromises
                 */
                CANPromiseManager(void * buffer, void * bitmask, size_t buffer_size);
                /**
                 * Takes a message and matches it to the current list of promises
                 * @param msg the incoming msg
                 * @return if a match was found
                 */
                bool match_message(const can::CAN_ROS_Message &msg);

                /**
                 * Creates a new promise for the given match
                 * @param mask the header mask
                 * @param filter the filter
                 * @return the promise object for the match
                 */
                CANPromise * match(can::CAN_Header mask, can::CAN_Header filter);
            private:
                void * buffer;
                void * bitmask;
                size_t buffer_size;
        };

        typedef void (*PromiseFn)(can::CAN_ROS_Message &, void * );
        class CANPromise {
            public:
                /**
                 * Function that specifies the callback function
                 * @param func the function
                 * @param data the data
                 * @return this object
                 */
                CANPromise * then(PromiseFn func, void * data);
                /**
                 * Called if there is an error on reading the promise
                 * @param func the function to call on an error
                 * @param data the data
                 * @return this object
                 */
                CANPromise * on_error(PromiseFn func, void * data);

                /**
                 * Block using the provided signal.
                 * Condition: signal is not otherwise triggered
                 *
                 * The promise is deleted once this call is made.
                 * You should not attempt to use this object after.
                 * @param signal the signal to use to manage this wait
                 * @return the message
                 */
                can::CAN_ROS_Message wait(const RtosSignalId &signal);

                /**
                 * Gets the value, blocks if its not ready
                 * @return the value
                 * @see wait
                 */
                can::CAN_ROS_Message get_value();

            private:



                friend class CANPromiseManager;
                /**
                 * Called by CANPromiseManager to give a promise waiting on the given
                 * mask and filter combo
                 * @param mask the header mask
                 * @param filter the header filter
                 */
                CANPromise(can::CAN_Header &mask, can::CAN_Header &filter);

                /**
                 * Checks if the header matches this promise.
                 * Called by CANPromiseManager
                 * @param header the header to check
                 * @return true on a match false otherwise
                 */
                bool matches(const can::CAN_Header &header);

                /**
                 * Called by CANPromiseManager on a match
                 * @param msg the message
                 * @param error if there is an error
                 */
                void trigger_match(const can::CAN_ROS_Message &msg, const bool error);

                PromiseFn then_fn;
                void * then_data;
                PromiseFn error_fn;
                void * error_data;
                bool error;

                RtosSignalId signal;
                RtosTaskId waiting_on;
                bool waiting = false;
                can::CAN_ROS_Message msg;

                const can::CAN_Header mask;
                const can::CAN_Header filter;
                /**
                 * Indicates the promise has returned
                 */
                bool ready = false;
        };
    }
}

#endif //PROJECT_CANPROMISE_H
