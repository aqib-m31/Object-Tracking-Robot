# Object Tracking Robot

An autonomous robot that uses an IR sensor for object detection and an ultrasonic sensor for distance measurement to continuously track and follow an object. The robot maintains a safe following distance, moving forward to pursue the object, reversing to avoid collisions, and stopping when no object is in range.

## Working Principle

1. **Sensors**
   - IR sensor (front-mounted) detects object presence via infrared reflection
   - IR sensor output: LOW = object detected (within ~2–20 cm), HIGH = no object
   - HC-SR04 ultrasonic sensor measures precise distance to the detected object

2. **Control Logic (Threshold-Based)**
   - **Object detected + distance ≤ 7 cm → Reverse** (too close, avoid collision)
   - **Object detected + distance ≤ 17 cm → Move forward** (maintain following distance)
   - **Object detected + distance > 17 cm → Stop** (object out of follow range)
   - **No object detected → Stop** (await re-detection)

3. **Motor Control**
   - Two-wheel drive (2WD) controlled via L298N motor driver
   - PWM speed: 120 when moving forward, 100 when reversing
   - Both motors run at equal speed for straight-line tracking

### Pin Configuration

| Component                    | Pin        |
| ---------------------------- | ---------- |
| IR Sensor (OUT)              | Digital 2  |
| Ultrasonic Trigger           | Digital 3  |
| Ultrasonic Echo              | Digital 4  |
| IN4 (Left motor direction)   | Digital 5  |
| IN3 (Left motor direction)   | Digital 6  |
| IN2 (Right motor direction)  | Digital 7  |
| IN1 (Right motor direction)  | Digital 8  |
| ENA (Right motor speed, PWM) | Digital 9  |
| ENB (Left motor speed, PWM)  | Digital 10 |

## Problems Faced

- **IR Sensor False Positives**: Ambient light caused erroneous detections. Resolved by adjusting the onboard potentiometer to restrict detection range to 2–20 cm and conducting tests under controlled indoor lighting.
- **Sensor Alignment**: Ensuring the IR and ultrasonic sensors were co-aligned at the front was critical — misalignment caused the sensors to detect different objects, breaking the tracking logic.
- **Threshold Tuning**: Initial distance thresholds caused oscillation (robot continuously switching between forward and reverse). Carefully calibrating `TOO_CLOSE` (7 cm) and `FOLLOW_DISTANCE` (17 cm) reduced instability.
- **Ultrasonic Noise on Fast-Moving Objects**: Rapid object movements occasionally produced invalid readings (0 cm). The NewPing library's built-in filtering helped minimize spurious measurements.

## Assumptions

- The robot starts in the presence of or facing a detectable object
- The tracking surface is flat and provides sufficient traction for consistent movement
- The target object has a surface reflective enough for IR detection
- The environment has stable, controlled lighting to prevent IR interference
- The robot tracks in a single forward-facing direction only (no lateral or rotational tracking)
- No other obstacles exist in the robot's path during tracking
