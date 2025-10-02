SUMMARY             = "DHT22 Temperature & Humidity Kernel Driver"
DESCRIPTION         = "Linux kernel driver for DHT22 temperature and humidity sensor"
LICENSE             = "MIT"
LIC_FILES_CHKSUM    = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit module

SRC_URI = "file://dht22.c \
           file://dht22.h \
           file://Makefile"

S = "${WORKDIR}"
MODULE_NAME = "dht22"

do_compile() 
{
    oe_runmake -C ${STAGING_KERNEL_DIR} M=${S} modules
}

do_install() 
{
    install -d ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/misc
    install -m 0644 ${MODULE_NAME}.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/misc/
    install -d ${D}/etc/modules-load.d
    echo "${MODULE_NAME}" > ${D}/etc/modules-load.d/${MODULE_NAME}.conf
}

FILES:${PN} += "/etc/modules-load.d/${MODULE_NAME}.conf"
RPROVIDES:${PN} += "kernel-module-${MODULE_NAME}"
