/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

using namespace imu_kvh_1750_mixed;

int AuxImuDriver::extractPacket(uint8_t const *buffer, size_t buffer_size) const {

	static bool found_start = false;

	char str[32] = "";
	strncpy(str, (const char*)buffer, buffer_size);

	if (!found_start){
		char *pch = strchr(str,'<');

		if (pch) {
			int len = (pch - str);
			found_start = true;
			return -len;
		}
	}

	if (found_start) {

		char *pch = strchr((char*)str,'>');
		if (pch){
			int len = (pch - (char*)str) + 1;
			found_start = false;
			return len;
		}

		return 0;
	}

	found_start = false;
	return -buffer_size;

}

void AuxImuDriver::parseMessage(uint8_t const* buffer, size_t size) {

	char str[32] = "";

	strncpy(str, (const char*)buffer, size);
	//printf("%s\n", str);
	sscanf(str, "<%f>", &gyro_x);
	//printf("%f\n", gyro_x);

}

Task::Task(std::string const& name)
    : TaskBase(name)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine)
{
}

Task::~Task()
{
}

bool Task::configureHook()
{
    if (! TaskBase::configureHook())
        return false;

    std::cout << _device.value() << std::endl;
	aux_imu_driver.reset(new AuxImuDriver());
	aux_imu_driver->setReadTimeout(base::Time::fromSeconds(_timeout.value()));
	aux_imu_driver->openURI(_device.value());

    return true;
}

bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;
    return true;
}

void Task::updateHook()
{
    TaskBase::updateHook();

    //Get data from the Gy-80 imu.
	try {
		std::vector<uint8_t> buffer;
		buffer.resize(16);
		aux_imu_driver->readPacket(&buffer[0], buffer.size());
		aux_imu_driver->parseMessage(&buffer[0],buffer.size());

		_kvh_samples.read(imu_mix);

		imu_mix.gyro[0] = -(aux_imu_driver->gyro_x * DPS2RAD);

		/** Output **/
	    _mix_samples.write(imu_mix);


	} catch (iodrivers_base::TimeoutError ex){
		std::cout << ex.what() << std::endl;
	}
}

void Task::errorHook()
{
    TaskBase::errorHook();
}
void Task::stopHook()
{
    TaskBase::stopHook();
}
void Task::cleanupHook()
{
    TaskBase::cleanupHook();
}
