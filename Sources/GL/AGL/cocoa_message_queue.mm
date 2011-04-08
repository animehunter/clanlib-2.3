/*
 **  ClanLib SDK
 **  Copyright (c) 1997-2011 The ClanLib Team
 **
 **  This software is provided 'as-is', without any express or implied
 **  warranty.  In no event will the authors be held liable for any damages
 **  arising from the use of this software.
 **
 **  Permission is granted to anyone to use this software for any purpose,
 **  including commercial applications, and to alter it and redistribute it
 **  freely, subject to the following restrictions:
 **
 **  1. The origin of this software must not be misrepresented; you must not
 **     claim that you wrote the original software. If you use this software
 **     in a product, an acknowledgment in the product documentation would be
 **     appreciated but is not required.
 **  2. Altered source versions must be plainly marked as such, and must not be
 **     misrepresented as being the original software.
 **  3. This notice may not be removed or altered from any source distribution.
 **
 **  Note: Some of the libraries ClanLib may link to may have additional
 **  requirements or restrictions.
 **
 **  File Author(s):
 **
 **    Magnus Norddahl
 */

#include "GL/precomp.h"
#include "cocoa_message_queue.h"

CL_CocoaMessageQueue *CL_CocoaMessageQueue::get()
{
    CL_MutexSection mutex_lock(&mutex);
    if (instance == 0)
        instance = new CL_CocoaMessageQueue();
    return instance;
}

CL_CocoaMessageQueue::CL_CocoaMessageQueue()
{
    enable_cocoa_threadsafety();
    //thread.start(this, &CL_CocoaMessageQueue::worker_main);
}

CL_CocoaMessageQueue::~CL_CocoaMessageQueue()
{
    CL_MutexSection mutex_lock(&mutex);
    instance = 0;
}

void CL_CocoaMessageQueue::enable_cocoa_threadsafety()
{
    // Cocoa enables thread safety for all threads when the first NSThread is started.
    // This function creates a single NS thread that does nothing to enable it.
    NSThread *ns_thread = [[NSThread alloc] init];
    [ns_thread start];
    [ns_thread release];
}

/*
void CL_CocoaMessageQueue::process_messages()
{
    CFRunLoopRef runloop = CFRunLoopGetCurrent();
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
}
*/
void CL_CocoaMessageQueue::worker_main()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    CFRunLoopRef runloop = CFRunLoopGetMain();
    CFRunLoopObserverContext context = { 0 };
    context.info = this;
    CFRunLoopObserverRef observer = CFRunLoopObserverCreate(kCFAllocatorDefault, kCFRunLoopAfterWaiting, FALSE, 0, &CL_CocoaMessageQueue::static_on_runloop_after_waiting, &context);
    CFRunLoopAddObserver(runloop, observer, kCFRunLoopDefaultMode);
    
    UIApplicationMain(0, 0, nil, nil/*@"CL_CocoaApplicationDelegate"*/);
    
    CFRunLoopRemoveObserver(runloop, observer, kCFRunLoopDefaultMode);
    [pool release];
    
    delete this;
}

void CL_CocoaMessageQueue::wakeup_worker()
{
    CFRunLoopRef runloop = CFRunLoopGetMain();
    CFRunLoopWakeUp(runloop);
}

void CL_CocoaMessageQueue::on_runloop_after_waiting()
{
    
}

void CL_CocoaMessageQueue::static_on_runloop_after_waiting(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void *info)
{
    CL_CocoaMessageQueue *self = reinterpret_cast<CL_CocoaMessageQueue*>(info);
    self->on_runloop_after_waiting();
}

CL_Mutex CL_CocoaMessageQueue::mutex;
CL_CocoaMessageQueue *CL_CocoaMessageQueue::instance = 0;
