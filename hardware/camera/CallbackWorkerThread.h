/*
 * Copyright (C) 2019, The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _THREAD_STD_H
#define _THREAD_STD_H

#include <thread>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <sys/time.h>

#include <hardware/camera.h>
#include <hardware/camera2.h>

#define CB_TYPE_NONE    0
#define CB_TYPE_NOTIFY  1
#define CB_TYPE_DATA    2

struct WorkerMessage {
    /* Worker callback type */
    int32_t CbType;

    /* Callback data */
    int32_t msg_type;
    const camera_memory_t *data;
    unsigned int index;
    camera_frame_metadata_t *metadata;
    void *user;
    int32_t ext1;
    int32_t ext2;
};

struct CallbackData {
    camera_notify_callback NewUserNotifyCb;
    camera_data_callback NewUserDataCb;
};

struct ThreadMsg;

class CallbackWorkerThread {
public:
    CallbackWorkerThread();
    ~CallbackWorkerThread();

    /* Creates our worker, returns true on success */
    bool CreateThread();

    /* Exits the worker thread */
    void ExitThread();

    /* Sends a new callback to our worker thread */
    void AddCallback(const WorkerMessage* data);

    /* Sets the callback function pointers for our worker to call */
    void SetCallbacks(const CallbackData* data);

    /* Clears the worker message queue */
    void ClearCallbacks(void);

private:
    CallbackWorkerThread(const CallbackWorkerThread&);
    CallbackWorkerThread& operator=(const CallbackWorkerThread&);

    long long GetTimestamp();

    /* Entry point for the worker thread */
    void Process();

    std::thread* m_thread;
    std::queue<ThreadMsg*> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    const char* m_name;
};

#endif
