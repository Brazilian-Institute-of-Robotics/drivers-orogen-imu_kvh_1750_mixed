/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef IMU_KVH_1750_MIXED_TASK_TASK_HPP
#define IMU_KVH_1750_MIXED_TASK_TASK_HPP

#include <iodrivers_base/Driver.hpp>
#include "imu_kvh_1750_mixed/TaskBase.hpp"


namespace imu_kvh_1750_mixed {

	static const double DPS2RAD = 0.0174532925; /** Factor to convert degree per second to radian per second**/

	class AuxImuDriver: public iodrivers_base::Driver
	{
	public:
		static const int MAX_BUFFER_SIZE = 64;
		float gyro_x = 0.0;

		AuxImuDriver() : iodrivers_base::Driver(MAX_BUFFER_SIZE) {}
		virtual ~AuxImuDriver() {}

		int extractPacket(uint8_t const *buffer, size_t buffer_size) const;
		void parseMessage(uint8_t const* buffer, size_t size);
	};

    class Task : public TaskBase
    {
	friend class TaskBase;
    protected:
		boost::shared_ptr<AuxImuDriver> aux_imu_driver;
    public:
		base::samples::IMUSensors imu_mix;

		Task(std::string const& name = "imu_kvh_1750_mixed::Task");

        Task(std::string const& name, RTT::ExecutionEngine* engine);

	    ~Task();

        bool configureHook();

        bool startHook();

        void updateHook();

        void errorHook();

        void stopHook();

        void cleanupHook();
    };
}

#endif

