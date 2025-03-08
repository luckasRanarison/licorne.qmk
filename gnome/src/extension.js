import {
  Extension,
  gettext as _,
} from "resource:///org/gnome/shell/extensions/extension.js";
import * as Main from "resource:///org/gnome/shell/ui/main.js";

import Indicator from "./ui/inidicator.js";
import { QmkHidMessenger } from "./protocol/subprocess.js";

export default class LicorneExtension extends Extension {
  enable() {
    const settings = this._getSettings();

    console.info(`starting with params: ${JSON.stringify(settings)}`);

    // @ts-ignore
    this._indicator = new Indicator();

    this._subprocess = QmkHidMessenger.init(
      settings.vendorId,
      settings.productId,
      (line) => this._onMessage(line),
    );

    this._intervalHandle = setInterval(() => {
      this._subprocess?.send({ type: "wpm" });
    }, settings.wpmInterval);

    Main.panel.addToStatusArea(this.uuid, this._indicator, 1, "left");
  }

  disable() {
    this._indicator?.destroy();
    this._subprocess?.destroy();

    if (this._intervalHandle) {
      clearInterval(this._intervalHandle);
    }
  }

  /**
   * @param params {HidMessage}
   */
  _onMessage(params) {
    if (params.type === "layer") {
      this._indicator?.update({ layer: params.payload });
    }

    if (params.type === "wpm") {
      this._indicator?.update({ wpm: params.payload });
    }
  }

  _getSettings() {
    const settings = this.getSettings();

    return {
      vendorId: settings.get_int("vendor-id"),
      productId: settings.get_int("product-id"),
      wpmInterval: settings.get_int("wpm-interval"),
    };
  }
}
