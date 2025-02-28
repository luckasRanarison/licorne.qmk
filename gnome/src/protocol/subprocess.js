import Gio from "gi://Gio";
import GLib from "gi://GLib";
import { PRODUCT_ID, VENDOR_ID } from "./constants.js";

class QmkHidMessenger {
  /**
   * @param process {import("gi://Gio").default.Subprocess}
   * @param stdout {import("gi://Gio").default.DataInputStream}
   */
  constructor(process, stdout) {
    this.process = process;
    this.stdout = stdout;
  }

  /**
   * @param onMessage {(line: import("./constants.js").HIDMessage) => void}
   */
  static init(onMessage) {
    const process = Gio.Subprocess.new(
      ["qmk-hid", "-v", `${VENDOR_ID}`, "-p", `${PRODUCT_ID}`],
      Gio.SubprocessFlags.STDOUT_PIPE | Gio.SubprocessFlags.STDERR_PIPE,
    );

    const stdoutPipe = process.get_stdout_pipe();

    if (!stdoutPipe) {
      throw new Error("Could not get stdout handle of the qmk-hid command");
    }

    const stdout = new Gio.DataInputStream({
      base_stream: stdoutPipe,
      close_base_stream: true,
    });

    const messenger = new QmkHidMessenger(process, stdout);
    messenger._listen(onMessage);

    return messenger;
  }

  destroy() {
    this.process.force_exit();
  }

  /**
   * @param onMessage {(line: import("./constants.js").HIDMessage) => void}
   */
  _listen(onMessage) {
    this.stdout.read_line_async(GLib.PRIORITY_LOW, null, (stream, result) => {
      try {
        if (!stream) {
          throw new Error("No stream from stdout");
        }

        const [line] = stream.read_line_finish_utf8(result);

        if (line !== null) {
          const [header, type, payload] = line.split(":");
          const message = { type, payload };

          if (header !== "msg") {
            throw new Error(`Invalid request: ${line}`);
          }

          console.debug(message);
          onMessage(message);
          this._listen(onMessage);
        }
      } catch (error) {
        console.error(error);
      }
    });
  }
}

export { QmkHidMessenger };
