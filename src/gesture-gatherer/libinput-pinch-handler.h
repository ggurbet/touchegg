/**
 * Copyright 2011 - 2020 José Expósito <jose.exposito89@gmail.com>
 *
 * This file is part of Touchégg.
 *
 * Touchégg is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License  as  published by  the  Free Software
 * Foundation,  either version 3 of the License,  or (at your option)  any later
 * version.
 *
 * Touchégg is distributed in the hope that it will be useful,  but  WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the  GNU General Public License  for more details.
 *
 * You should have received a copy of the  GNU General Public License along with
 * Touchégg. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GESTURE_GATHERER_LIBINPUT_PINCH_HANDLER_H_
#define GESTURE_GATHERER_LIBINPUT_PINCH_HANDLER_H_

#include <memory>

#include "gesture-controller/gesture-controller-delegate.h"
#include "gesture-gatherer/libinput-handler.h"
#include "gesture/gesture-direction.h"
#include "gesture/libinput-gesture.h"

/**
 * Data structure to save pinch state in a single place.
 */
struct LibinputPinchState {
  bool started = false;
  uint64_t startTimestamp = 0;
  double delta = 1;
  GestureDirection direction = GestureDirection::UNKNOWN;
  int percentage = 0;

  void reset() {
    started = false;
    startTimestamp = 0;
    delta = 1;
    direction = GestureDirection::UNKNOWN;
    percentage = 0;
  }
};

/**
 * Class to handle libinput pinch gestures on a touchpad:
 * https://wayland.freedesktop.org/libinput/doc/latest/gestures.html#swipe-gestures
 */
class LininputPinchHandler : public LininputHandler {
 public:
  explicit LininputPinchHandler(GestureControllerDelegate *gestureController)
      : LininputHandler(gestureController) {}

  /**
   * When the user starts a pinch, we still don't know the direction, so here we
   * just reset this->swipeState.
   * @param gesture Libinput specialized gesture.
   */
  void handlePinchBegin(std::unique_ptr<LibinputGesture> gesture);

  /**
   * Unlike swipe, a threshold is not required for this gesture.
   * The first update sends a begin event to the GestureControllerDelegate.
   * Consecutive updates send a update event to the GestureControllerDelegate.
   * @param gesture Libinput specialized gesture.
   */
  void handlePinchUpdate(std::unique_ptr<LibinputGesture> gesture);

  /**
   * Send a end event to the GestureControllerDelegate if a gesture was
   * detected.
   * @param gesture Libinput specialized gesture.
   */
  void handlePinchEnd(std::unique_ptr<LibinputGesture> gesture);

 private:
  LibinputPinchState state;

  /**
   * @returns The percentage (between 0 and 100) of the gesture animation.
   */
  int calculatePinchAnimationPercentage(GestureDirection direction,
                                        double delta) const;
};
#endif  // GESTURE_GATHERER_LIBINPUT_PINCH_HANDLER_H_
