# use newer wifi cards on jetson nano
if you want to use recent intel wifi cards that is not supported by linux-4.9.x, you have to backport the iwlwifi driver.  
for l4t >= 32.7.3, an additional patch may be required. see link [1].

[1] https://github.com/yoffy/jetson-nano-kernel/blob/master/backport-iwlwifi.patch
## install driver
```
# i prefer separate workdir
mkdir workdir
cd workdir
# clone repository
git clone --depth=1 -b release/core52 https://git.kernel.org/pub/scm/linux/kernel/git/iwlwifi/backport-iwlwifi.git
# configure
cd backport-iwlwifi
make defconfig-iwlwifi-public
# build
make -j$(nproc)
# install
sudo make modules_install
```
## install firmwares
if there are no iwlwifi-9260-* files in /lib/firmware, download and place them from [2].  
then, create missing firmware config file to make the driver happy.
```
echo -e '[IWL DEBUG CONFIG DATA]\nFW_DBG_PRESET=1' | sudo tee /lib/firmware/iwl-dbg-cfg.ini
```

[2] https://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git
