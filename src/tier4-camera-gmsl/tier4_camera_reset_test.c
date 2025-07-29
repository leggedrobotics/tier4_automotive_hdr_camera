/*
 * tier4_camera_reset_test.c - Test module for camera reset sysfs interface
 *
 * This module creates a test device to verify the camera reset sysfs
 * interface implementation without requiring actual camera hardware.
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

static struct platform_device *test_pdev;

static ssize_t
camera_reset_store(struct device *dev, struct device_attribute *attr,
		   const char *buf, size_t count)
{
	long long reset_cmd;
	int err;

	dev_info(dev, "%s: camera reset requested: %s\n", __func__, buf);

	err = kstrtoull(buf, 10, &reset_cmd);
	if (err)
		return err;

	/* Only trigger reset if value is 1 */
	if (reset_cmd == 1) {
		dev_info(dev, "%s: Performing camera reset (simulated)\n", __func__);
		/* Simulate reset operation */
		msleep(100);
		dev_info(dev, "%s: Camera reset completed (simulated)\n", __func__);
	} else {
		dev_warn(dev, "%s: Invalid reset command %lld (use 1 to reset)\n",
			 __func__, reset_cmd);
		return -EINVAL;
	}

	return count;
}

static ssize_t
camera_reset_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "Write 1 to trigger camera reset (test mode)\n");
}

static DEVICE_ATTR_RW(camera_reset);

static int test_probe(struct platform_device *pdev)
{
	int ret;

	dev_info(&pdev->dev, "Test camera reset module probe\n");

	ret = device_create_file(&pdev->dev, &dev_attr_camera_reset);
	if (ret) {
		dev_err(&pdev->dev, "Failed to create camera_reset sysfs file: %d\n", ret);
		return ret;
	}

	dev_info(&pdev->dev, "Camera reset sysfs interface created successfully\n");
	dev_info(&pdev->dev, "Test interface available at: /sys/devices/platform/%s/camera_reset\n", 
		 dev_name(&pdev->dev));

	return 0;
}

static int test_remove(struct platform_device *pdev)
{
	device_remove_file(&pdev->dev, &dev_attr_camera_reset);
	dev_info(&pdev->dev, "Test camera reset module removed\n");
	return 0;
}

static struct platform_driver test_driver = {
	.driver = {
		.name = "tier4_camera_reset_test",
	},
	.probe = test_probe,
	.remove = test_remove,
};

static int __init test_init(void)
{
	int ret;

	pr_info("tier4_camera_reset_test: Loading test module\n");

	ret = platform_driver_register(&test_driver);
	if (ret) {
		pr_err("tier4_camera_reset_test: Failed to register driver: %d\n", ret);
		return ret;
	}

	test_pdev = platform_device_register_simple("tier4_camera_reset_test", -1, NULL, 0);
	if (IS_ERR(test_pdev)) {
		ret = PTR_ERR(test_pdev);
		pr_err("tier4_camera_reset_test: Failed to register device: %d\n", ret);
		platform_driver_unregister(&test_driver);
		return ret;
	}

	pr_info("tier4_camera_reset_test: Test module loaded successfully\n");
	return 0;
}

static void __exit test_exit(void)
{
	platform_device_unregister(test_pdev);
	platform_driver_unregister(&test_driver);
	pr_info("tier4_camera_reset_test: Test module unloaded\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_DESCRIPTION("Test module for Tier4 camera reset interface");
MODULE_AUTHOR("Tier4");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.0");
