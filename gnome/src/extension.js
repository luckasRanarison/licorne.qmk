import {
  Extension,
  gettext as _,
} from "resource:///org/gnome/shell/extensions/extension.js";
import * as Main from "resource:///org/gnome/shell/ui/main.js";

import Indicator from "./ui/inidicator.js";
import { QmkHidMessenger } from "./protocol/subprocess.js";

export default class LicorneExtension extends Extension {
  enable() {
    const settings = this.getSettings();
    const vendorId = settings.get_int("vendor-id");
    const productId = settings.get_int("product-id");

    console.info(`Vendor ID: ${vendorId}, Product ID: ${productId}`);

    // @ts-ignore
    this._indicator = new Indicator();
    this._subprocess = QmkHidMessenger.init(vendorId, productId, (line) =>
      this._onMessage(line),
    );

    Main.panel.addToStatusArea(this.uuid, this._indicator, 1, "left");
  }

  disable() {
    this._indicator?.destroy();
    this._subprocess?.destroy();
  }

  /**
   * @param params {HidMessage}
   */
  _onMessage(params) {
    if (params.type === "layer") {
      this._indicator?.update(params.payload);
    }
  }
}
