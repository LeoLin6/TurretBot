# Import Libraries
import serial
import time

# Constants (defaults)
DEFAULT_PORT = 'COM4'
BAUDRATE = 115200
TIMEOUT = 0.1
DEFAULT_EXP_ONE_STEPS = 72
DEFAULT_STEP_DEGREES = 5

# -------------------- Arduino Communication --------------------

def send_command(command: str, value=None, delay=0):
    """Send a command and optional value to the Arduino."""
    arduino.write(f"{command}\n".encode())
    if value is not None:
        arduino.write(f"{value}\n".encode())
    if delay > 0:
        time.sleep(delay)

def calibrate():
    print("Running calibration sequence...")
    send_command("cali", 16, delay=7)
    print("Calibration complete. Moved to position 0.")

# -------------------- Experiment One --------------------

def experiment_one_audio():
    # Placeholder for audio playback
    time.sleep(3)

def experiment_one(num_steps, step_degrees):
    """Moves talking head in increments of N degrees, N times."""
    calibrate()
    for step in range(1, num_steps + 1):
        print(f"Movement step: {step}")
        send_command("pos", step_degrees, delay=5)
        experiment_one_audio()

# -------------------- Experiment Two --------------------

def experiment_two_audio():
    # Placeholder for WGN audio
    time.sleep(4)

def experiment_two(rps, loops):
    """Moves to +90, plays audio, then oscillates at user-defined RPS and loops."""
    for loop in range(loops):
        print(f"\n--- Loop {loop + 1} of {loops} ---")
        calibrate()

        print("Moving to +90 degrees...")
        send_command("pos", 90, delay=1)
        experiment_two_audio()

        print(f"Oscillating at {rps} RPS...")
        velocity = int(rps * 100)
        duration = int(1 / (2 * rps) * 100)

        send_command("vel", velocity)
        send_command(str(duration))  # Send as raw duration

        time.sleep(1 / (2 * rps))
        print("Moved to -90 degrees")

# -------------------- Main Interactive Setup --------------------

def main():
    # Port selection
    port_input = input(f"Enter COM port for Arduino [default: {DEFAULT_PORT}]: ").strip()
    port = port_input if port_input else DEFAULT_PORT

    try:
        global arduino
        arduino = serial.Serial(port=port, baudrate=BAUDRATE, timeout=TIMEOUT)
        time.sleep(2)  # Allow Arduino to initialize
        print(f"✅ Connected to Arduino on {port}")
    except serial.SerialException as e:
        print(f"❌ Failed to connect to Arduino on {port}: {e}")
        return

    # Experiment selection
    print("\nSelect an experiment to run:")
    print("1 - Experiment One")
    print("2 - Experiment Two")
    choice = input("Enter 1 or 2: ").strip()

    if choice == '1':
        # Customize experiment one
        print("\nExperiment One configuration:")
        use_custom = input(f"Change number of steps? [default: {DEFAULT_EXP_ONE_STEPS}] (y/n): ").strip().lower()
        if use_custom == 'y':
            while True:
                try:
                    num_steps = int(input("Enter number of movement steps: "))
                    if num_steps > 0:
                        break
                    else:
                        print("Must be a positive integer.")
                except ValueError:
                    print("Invalid input. Please enter an integer.")
        else:
            num_steps = DEFAULT_EXP_ONE_STEPS

        use_custom_deg = input(f"Change degrees per step? [default: {DEFAULT_STEP_DEGREES}] (y/n): ").strip().lower()
        if use_custom_deg == 'y':
            while True:
                try:
                    step_degrees = int(input("Enter degrees per step (e.g., 5): "))
                    if step_degrees > 0:
                        break
                    else:
                        print("Must be a positive number.")
                except ValueError:
                    print("Invalid input. Please enter an integer.")
        else:
            step_degrees = DEFAULT_STEP_DEGREES

        experiment_one(num_steps, step_degrees)

    elif choice == '2':
        # RPS
        while True:
            rps_input = input("Enter desired RPS (e.g., 0.1, 0.25, 0.5): ").strip()
            try:
                rps_value = float(rps_input)
                if rps_value > 0:
                    break
                else:
                    print("RPS must be a positive number.")
            except ValueError:
                print("Please enter a valid number.")

        # Loops
        while True:
            loops_input = input("Enter number of loops (e.g., 2, 5, 10): ").strip()
            try:
                loops_value = int(loops_input)
                if loops_value > 0:
                    break
                else:
                    print("Number of loops must be a positive integer.")
            except ValueError:
                print("Please enter a valid integer.")

        experiment_two(rps_value, loops_value)

    else:
        print("Invalid choice. Exiting.")

# -------------------- Run Program --------------------
if __name__ == "__main__":
    main()
