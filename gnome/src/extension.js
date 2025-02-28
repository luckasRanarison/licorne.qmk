import {
  Extension,
  gettext as _,
} from "resource:///org/gnome/shell/extensions/extension.js";
import * as Main from "resource:///org/gnome/shell/ui/main.js";
import Indicator from "./ui/inidicator.js";
import { QmkHidMessenger } from "./protocol/subprocess.js";

export default class LicorneExtension extends Extension {
  enable() {
    // @ts-ignore
    this._indicator = new Indicator();
    this._subprocess = QmkHidMessenger.init((line) => this._onMessage(line));

    Main.panel.addToStatusArea(this.uuid, this._indicator, 0, "left");
  }

  disable() {
    this._indicator?.destroy();
    this._subprocess?.destroy();
  }

  /**
   * @param params {import("./protocol/constants.js").HIDMessage}
   */
  _onMessage(params) {
    if (params.type === "layer") {
      this._indicator?.upateText(params.payload);
    }
  }
}
