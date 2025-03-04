import St from "gi://St";
import Clutter from "gi://Clutter";
import GObject from "gi://GObject";

import * as PanelMenu from "resource:///org/gnome/shell/ui/panelMenu.js";
import { LAYERS } from "../utils/constants.js";

class _Indicator extends PanelMenu.Button {
  _init() {
    super._init(1, "Licorne indicator");

    this._label = new St.Label({ text: "-- BASE --" });

    const container = new St.BoxLayout({
      y_align: Clutter.ActorAlign.CENTER,
    });
    container.add_child(this._label);

    this.add_child(container);
  }

  /**
   * @param layer {string}
   */
  update(layer) {
    if (LAYERS.has(layer)) {
      this._label?.set_text(`-- ${layer} --`);
    } else {
      console.error("Unknown layer: ", layer);
    }
  }
}

const Indicator = GObject.registerClass(_Indicator);

export default Indicator;
