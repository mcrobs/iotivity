/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
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
 *
 ******************************************************************/
#ifndef SMARTHOME_API_CLIENT_REMOTEMEDIASOURCEINPUT_H_
#define SMARTHOME_API_CLIENT_REMOTEMEDIASOURCEINPUT_H_
#include <RemoteMediaSourceResource.h>

namespace OIC
{
    namespace Service
    {
        namespace SH
        {
            class RemoteMediaSourceInputResourceDelegate: public RemoteMediaSourceResourceDelegate
            {
            public:
                virtual ~RemoteMediaSourceInputResourceDelegate() {}
                virtual void onGetMediaSource(std::list<MediaSource> sources) = 0;
                virtual void onChangeMediaSource(std::list<MediaSource> sources) = 0;
            };


            class RemoteMediaSourceInputResource: public RemoteMediaSourceResource
            {
            public:
                virtual ~RemoteMediaSourceInputResource();

                void getMediaSources();
                void changeMediaSources(std::list<MediaSource> sources);
                void setDelegate(RemoteMediaSourceInputResourceDelegate *delegate);

            protected:
                RemoteMediaSourceInputResource();
            };
        }
    }
}
#endif /* SMARTHOME_API_CLIENT_REMOTEMEDIASOURCEINPUT_H_ */