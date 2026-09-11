State Machine Description (English)
States

MOTOR_STOPPED

Motor is stopped.
Start command initiates the start sequence.

MOTOR_STARTING

Start output is energized.
System waits for Run Feedback.
If feedback is received within 5 seconds, the state changes to RUNNING.
If timeout expires, the state changes to FAULT.

MOTOR_RUNNING

Motor is confirmed running.
Stop command initiates the stop sequence.

MOTOR_STOPPING

Start output is de-energized.
System waits for feedback to disappear.
If feedback is removed within 5 seconds, the state changes to STOPPED.
If timeout expires, the state changes to FAULT.

MOTOR_FAULT

Motor output is disabled.
Manual reset is required.
//-----------------------------------------------
cmdStart      -> Start pushbutton / PLC command
cmdStop       -> Stop pushbutton / PLC command
runFeedback   -> Auxiliary contact of motor starter
motorOutput   -> Contactor coil command
resetFault    -> Fault reset pushbutton
