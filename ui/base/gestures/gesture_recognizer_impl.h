// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GESTURES_GESTURE_RECOGNIZER_IMPL_H_
#define UI_BASE_GESTURES_GESTURE_RECOGNIZER_IMPL_H_
#pragma once

#include <map>
#include <queue>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/events.h"
#include "ui/base/gestures/gesture_recognizer.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/point.h"

namespace ui {
class TouchEvent;
class GestureEvent;
class GestureSequence;
class GestureConsumer;
class GestureEventHelper;

class UI_EXPORT GestureRecognizerImpl : public GestureRecognizer {
 public:
  explicit GestureRecognizerImpl(GestureEventHelper* helper);
  virtual ~GestureRecognizerImpl();

  // Checks if this finger is already down, if so, returns the current target.
  // Otherwise, returns NULL.
  virtual GestureConsumer* GetTouchLockedTarget(TouchEvent* event) OVERRIDE;

  // Returns the target of the touches the gesture is composed of.
  virtual GestureConsumer* GetTargetForGestureEvent(
      GestureEvent* event) OVERRIDE;

  virtual GestureConsumer* GetTargetForLocation(
      const gfx::Point& location) OVERRIDE;

  // Each touch which isn't targeted to capturer results in a touch
  // cancel event. These touches are then targeted to
  // gesture_consumer_ignorer.
  virtual void CancelNonCapturedTouches(GestureConsumer* capturer) OVERRIDE;

 protected:
  virtual GestureSequence* CreateSequence(GestureEventHelper* helper);
  virtual GestureSequence* GetGestureSequenceForConsumer(GestureConsumer* c);

 private:
  // Overridden from GestureRecognizer
  virtual Gestures* ProcessTouchEventForGesture(
      const TouchEvent& event,
      ui::TouchStatus status,
      GestureConsumer* target) OVERRIDE;
  virtual void QueueTouchEventForGesture(GestureConsumer* consumer,
                                         const TouchEvent& event) OVERRIDE;
  virtual Gestures* AdvanceTouchQueue(GestureConsumer* consumer,
                                      bool processed) OVERRIDE;
  virtual void FlushTouchQueue(GestureConsumer* consumer) OVERRIDE;

  typedef std::queue<TouchEvent*> TouchEventQueue;
  std::map<GestureConsumer*, TouchEventQueue*> event_queue_;
  std::map<GestureConsumer*, GestureSequence*> consumer_sequence_;

  // Both touch_id_target_ and touch_id_target_for_gestures_
  // map a touch-id to its target window.
  // touch_ids are removed from touch_id_target_ on ET_TOUCH_RELEASE
  // and ET_TOUCH_CANCEL. touch_id_target_for_gestures_ never has touch_ids
  // removed.
  std::map<int, GestureConsumer*> touch_id_target_;
  std::map<int, GestureConsumer*> touch_id_target_for_gestures_;

  // Touches cancelled by touch capture are routed to the
  // gesture_consumer_ignorer_.
  scoped_ptr<GestureConsumer> gesture_consumer_ignorer_;
  GestureEventHelper* helper_;

  DISALLOW_COPY_AND_ASSIGN(GestureRecognizerImpl);
};

}  // namespace ui

#endif  // UI_BASE_GESTURES_GESTURE_RECOGNIZER_IMPL_H_
