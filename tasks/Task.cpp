/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

using namespace imu_kvh_1750_mixed;

int AuxImuDriver::extractPacket(uint8_t const *buffer, size_t buffer_size) const {

    if (buffer[0] != '<') {
        return -1;
    }

    int offset = 1;

    while (offset < buffer_size) {
        if (buffer[offset++] == '>') {
            return offset;
        }
    }

	return 0;

}

void AuxImuDriver::parseMessage(uint8_t const* buffer, size_t size) {
	sscanf((const char*)buffer, "<%f>", &gyro_x);
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

    /* Get data from the Gy-80 imu. */
	std::vector<uint8_t> buffer;
	buffer.resize(AuxImuDriver::MAX_BUFFER_SIZE);

	size_t size = aux_imu_driver->readPacket(&buffer[0], buffer.size());
	aux_imu_driver->parseMessage(&buffer[0], size);
	_kvh_samples.read(imu_mix);

	imu_mix.gyro[0] = -(aux_imu_driver->gyro_x * DPS2RAD);

	/** Output **/
	_mix_samples.write(imu_mix);
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
