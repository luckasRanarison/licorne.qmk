import Gtk from "gi://Gtk";
import Adw from "gi://Adw";
import Gio from "gi://Gio";
import {
  ExtensionPreferences,
  gettext as _,
} from "resource:///org/gnome/Shell/Extensions/js/extensions/prefs.js";

export default class GnomeRectanglePreferences extends ExtensionPreferences {
  fillPreferencesWindow(window) {
    this._settings = this.getSettings();

    const page = new Adw.PreferencesPage({
      title: _("General"),
      iconName: "dialog-information-symbolic",
    });

    const idsGroup = new Adw.PreferencesGroup({
      title: _("IDs"),
      description: _("Configure the keyboard HID IDs"),
    });

    const vendorId = new Adw.SpinRow({
      title: _("Vendor ID"),
      subtitle: _("Keyboard vendor ID"),
      adjustment: new Gtk.Adjustment({
        lower: 0,
        upper: 0xffff,
        step_increment: 1,
      }),
    });

    const productId = new Adw.SpinRow({
      title: _("Product ID"),
      subtitle: _("Keyboard product ID"),
      adjustment: new Gtk.Adjustment({
        lower: 0,
        upper: 0xffff,
        step_increment: 1,
      }),
    });

    idsGroup.add(vendorId);
    idsGroup.add(productId);

    page.add(idsGroup);
    window.add(page);

    this._settings.bind(
      "vendor-id",
      vendorId,
      "value",
      Gio.SettingsBindFlags.DEFAULT,
    );
    this._settings.bind(
      "product-id",
      productId,
      "value",
      Gio.SettingsBindFlags.DEFAULT,
    );

    return Promise.resolve();
  }
}
