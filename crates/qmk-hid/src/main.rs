use std::{thread, time::Duration};

use clap::Parser;
use hidapi::{HidApi, HidError, HidResult};

#[derive(Parser)]
#[command(version, about)]
struct Cli {
    #[clap(short, long)]
    vendor_id: u16,

    #[clap(short, long)]
    product_id: u16,

    #[clap(default_value_t = 0x0061)]
    usage_id: u16,

    #[clap(default_value_t = 0xFF60)]
    usage_page: u16,

    #[clap(default_value_t = 5000)]
    poll_interval: u64,
}

impl Cli {
    const HID_MSG_LEN: usize = 32;

    fn read_device(&self, api: &HidApi) -> HidResult<()> {
        let device = api
            .device_list()
            .find(|device| {
                device.vendor_id() == self.vendor_id
                    && device.product_id() == self.product_id
                    && device.usage_page() == self.usage_page
                    && device.usage() == self.usage_id
            })
            .ok_or(HidError::HidApiErrorEmpty)?
            .open_device(&api)?;

        let mut buffer = [0u8; Self::HID_MSG_LEN];

        loop {
            let bytes_read = device.read(&mut buffer)?;
            let payload = String::from_utf8_lossy(&buffer[..bytes_read]);

            println!("{payload}");
        }
    }
}

fn main() -> HidResult<()> {
    let cli = Cli::parse();
    let api = HidApi::new()?;
    let interval = Duration::from_millis(cli.poll_interval);

    loop {
        match cli.read_device(&api) {
            Err(HidError::HidApiErrorEmpty) => {
                eprintln!("Error: The device was not found, retrying...")
            }
            Err(_) => {
                eprintln!("Error: An error occured, reconnecting...")
            }
            Ok(_) => {}
        };

        thread::sleep(interval);
    }
}
