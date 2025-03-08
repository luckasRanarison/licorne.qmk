import Gio from "gi://Gio";
import GLib from "gi://GLib";

class QmkHidMessenger {
  /**
   * @param process {import("gi://Gio").default.Subprocess}
   */
  constructor(process) {
    const stdoutPipe = process.get_stdout_pipe();
    const stdinPipe = process.get_stdin_pipe();

    if (!stdoutPipe || !stdinPipe) {
      throw new Error("Could not get stdout handle of the qmk-hid command");
    }

    this.stdout = new Gio.DataInputStream({
      base_stream: stdoutPipe,
      close_base_stream: true,
    });

    this.stdin = stdinPipe;
    this.process = process;
  }

  /**
   * @param vendorId {number}
   * @param productId {number}
   * @param onMessage {(line: HidMessage) => void}
   */
  static init(vendorId, productId, onMessage) {
    const process = Gio.Subprocess.new(
      ["qmk-hid", "-v", `${vendorId}`, "-p", `${productId}`],
      Gio.SubprocessFlags.STDOUT_PIPE | Gio.SubprocessFlags.STDIN_PIPE,
    );

    const messenger = new QmkHidMessenger(process);
    messenger._listen(onMessage);

    return messenger;
  }

  /**
   * @param params {HidRequest}
   */
  send({ type, payload }) {
    const cancellable = new Gio.Cancellable();
    const message = `req:${type}${payload ? ":" + payload : ""}\n`;

    this.stdin.write_all(message, cancellable);
  }

  destroy() {
    this.process.force_exit();
    this.process.wait();
  }

  /**
   * @param onMessage {(line: HidMessage) => void}
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
