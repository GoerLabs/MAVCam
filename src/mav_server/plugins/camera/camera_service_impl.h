#pragma once

// TODO need auto generate
#include <atomic>
#include <cmath>
#include <future>
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

#include "base/log.h"
#include "camera/camera.grpc.pb.h"
#include "camera/camera.pb.h"
#include "plugins/camera/camera.h"

namespace mav {

class CameraServiceImpl final : public mavsdk::rpc::camera::CameraService::Service {
public:
    CameraServiceImpl(std::shared_ptr<Camera> plugin) : _plugin(plugin) {}

    template <typename ResponseType>
    void fillResponseWithResult(ResponseType *response, mav::Camera::Result &result) const {
        auto rpc_result = translateToRpcResult(result);

        auto *rpc_camera_result = new mavsdk::rpc::camera::CameraResult();
        rpc_camera_result->set_result(rpc_result);
        std::stringstream ss;
        ss << result;
        rpc_camera_result->set_result_str(ss.str());

        response->set_allocated_camera_result(rpc_camera_result);
    }

    static mavsdk::rpc::camera::Mode translateToRpcMode(const mav::Camera::Mode &mode) {
        switch (mode) {
            default:
                base::LogError() << "Unknown mode enum value: " << static_cast<int>(mode);
            // FALLTHROUGH
            case mav::Camera::Mode::Unknown:
                return mavsdk::rpc::camera::MODE_UNKNOWN;
            case mav::Camera::Mode::Photo:
                return mavsdk::rpc::camera::MODE_PHOTO;
            case mav::Camera::Mode::Video:
                return mavsdk::rpc::camera::MODE_VIDEO;
        }
    }

    static mav::Camera::Mode translateFromRpcMode(const mavsdk::rpc::camera::Mode mode) {
        switch (mode) {
            default:
                base::LogError() << "Unknown mode enum value: " << static_cast<int>(mode);
            // FALLTHROUGH
            case mavsdk::rpc::camera::MODE_UNKNOWN:
                return mav::Camera::Mode::Unknown;
            case mavsdk::rpc::camera::MODE_PHOTO:
                return mav::Camera::Mode::Photo;
            case mavsdk::rpc::camera::MODE_VIDEO:
                return mav::Camera::Mode::Video;
        }
    }

    static mavsdk::rpc::camera::PhotosRange translateToRpcPhotosRange(
        const mav::Camera::PhotosRange &photos_range) {
        switch (photos_range) {
            default:
                base::LogError() << "Unknown photos_range enum value: "
                                 << static_cast<int>(photos_range);
            // FALLTHROUGH
            case mav::Camera::PhotosRange::All:
                return mavsdk::rpc::camera::PHOTOS_RANGE_ALL;
            case mav::Camera::PhotosRange::SinceConnection:
                return mavsdk::rpc::camera::PHOTOS_RANGE_SINCE_CONNECTION;
        }
    }

    static mav::Camera::PhotosRange translateFromRpcPhotosRange(
        const mavsdk::rpc::camera::PhotosRange photos_range) {
        switch (photos_range) {
            default:
                base::LogError() << "Unknown photos_range enum value: "
                                 << static_cast<int>(photos_range);
            // FALLTHROUGH
            case mavsdk::rpc::camera::PHOTOS_RANGE_ALL:
                return mav::Camera::PhotosRange::All;
            case mavsdk::rpc::camera::PHOTOS_RANGE_SINCE_CONNECTION:
                return mav::Camera::PhotosRange::SinceConnection;
        }
    }

    static mavsdk::rpc::camera::CameraResult::Result translateToRpcResult(
        const mav::Camera::Result &result) {
        switch (result) {
            default:
                base::LogError() << "Unknown result enum value: " << static_cast<int>(result);
            // FALLTHROUGH
            case mav::Camera::Result::Unknown:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_UNKNOWN;
            case mav::Camera::Result::Success:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_SUCCESS;
            case mav::Camera::Result::InProgress:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_IN_PROGRESS;
            case mav::Camera::Result::Busy:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_BUSY;
            case mav::Camera::Result::Denied:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_DENIED;
            case mav::Camera::Result::Error:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_ERROR;
            case mav::Camera::Result::Timeout:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_TIMEOUT;
            case mav::Camera::Result::WrongArgument:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_WRONG_ARGUMENT;
            case mav::Camera::Result::NoSystem:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_NO_SYSTEM;
            case mav::Camera::Result::ProtocolUnsupported:
                return mavsdk::rpc::camera::CameraResult_Result_RESULT_PROTOCOL_UNSUPPORTED;
        }
    }

    static mavsdk::rpc::camera::Information::CameraCapFlags translateToRpcCameraCapFlags(
        const mav::Camera::Information::CameraCapFlags &camera_cap_flags) {
        switch (camera_cap_flags) {
            default:
                base::LogError() << "Unknown camera_cap_flags enum value: "
                                 << static_cast<int>(camera_cap_flags);
            // FALLTHROUGH
            case mav::Camera::Information::CameraCapFlags::CaptureVideo:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_CAPTURE_VIDEO;
            case mav::Camera::Information::CameraCapFlags::CaptureImage:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_CAPTURE_IMAGE;
            case mav::Camera::Information::CameraCapFlags::HasModes:
                return mavsdk::rpc::camera::Information_CameraCapFlags_CAMERA_CAP_FLAGS_HAS_MODES;
            case mav::Camera::Information::CameraCapFlags::CanCaptureImageInVideoMode:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_CAN_CAPTURE_IMAGE_IN_VIDEO_MODE;
            case mav::Camera::Information::CameraCapFlags::CanCaptureVideoInImageMode:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_CAN_CAPTURE_VIDEO_IN_IMAGE_MODE;
            case mav::Camera::Information::CameraCapFlags::HasImageSurveyMode:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_HAS_IMAGE_SURVEY_MODE;
            case mav::Camera::Information::CameraCapFlags::HasBasicZoom:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_HAS_BASIC_ZOOM;
            case mav::Camera::Information::CameraCapFlags::HasBasicFocus:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_HAS_BASIC_FOCUS;
            case mav::Camera::Information::CameraCapFlags::HasVideoStream:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_HAS_VIDEO_STREAM;
            case mav::Camera::Information::CameraCapFlags::HasTrackingPoint:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_HAS_TRACKING_POINT;
            case mav::Camera::Information::CameraCapFlags::HasTrackingRectangle:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_HAS_TRACKING_RECTANGLE;
            case mav::Camera::Information::CameraCapFlags::HasTrackingGeoStatus:
                return mavsdk::rpc::camera::
                    Information_CameraCapFlags_CAMERA_CAP_FLAGS_HAS_TRACKING_GEO_STATUS;
        }
    }

    static std::unique_ptr<mavsdk::rpc::camera::Information> translateToRpcInformation(
        const mav::Camera::Information &information) {
        auto rpc_obj = std::make_unique<mavsdk::rpc::camera::Information>();

        rpc_obj->set_vendor_name(information.vendor_name);

        rpc_obj->set_model_name(information.model_name);

        rpc_obj->set_firmware_version(information.firmware_version);

        rpc_obj->set_focal_length_mm(information.focal_length_mm);

        rpc_obj->set_horizontal_sensor_size_mm(information.horizontal_sensor_size_mm);

        rpc_obj->set_vertical_sensor_size_mm(information.vertical_sensor_size_mm);

        rpc_obj->set_horizontal_resolution_px(information.horizontal_resolution_px);

        rpc_obj->set_vertical_resolution_px(information.vertical_resolution_px);

        rpc_obj->set_lens_id(information.lens_id);

        rpc_obj->set_definition_file_version(information.definition_file_version);

        rpc_obj->set_definition_file_uri(information.definition_file_uri);

        for (const auto &elem : information.camera_cap_flags) {
            rpc_obj->add_camera_cap_flags(translateToRpcCameraCapFlags(elem));
        }

        return rpc_obj;
    }

    static std::unique_ptr<mavsdk::rpc::camera::VideoStreamSettings>
    translateToRpcVideoStreamSettings(
        const mav::Camera::VideoStreamSettings &video_stream_settings) {
        auto rpc_obj = std::make_unique<mavsdk::rpc::camera::VideoStreamSettings>();

        rpc_obj->set_frame_rate_hz(video_stream_settings.frame_rate_hz);

        rpc_obj->set_horizontal_resolution_pix(video_stream_settings.horizontal_resolution_pix);

        rpc_obj->set_vertical_resolution_pix(video_stream_settings.vertical_resolution_pix);

        rpc_obj->set_bit_rate_b_s(video_stream_settings.bit_rate_b_s);

        rpc_obj->set_rotation_deg(video_stream_settings.rotation_deg);

        rpc_obj->set_uri(video_stream_settings.uri);

        rpc_obj->set_horizontal_fov_deg(video_stream_settings.horizontal_fov_deg);

        return rpc_obj;
    }

    static mavsdk::rpc::camera::VideoStreamInfo::VideoStreamStatus translateToRpcVideoStreamStatus(
        const mav::Camera::VideoStreamInfo::VideoStreamStatus &video_stream_status) {
        switch (video_stream_status) {
            default:
                base::LogError() << "Unknown video_stream_status enum value: "
                                 << static_cast<int>(video_stream_status);
            // FALLTHROUGH
            case mav::Camera::VideoStreamInfo::VideoStreamStatus::NotRunning:
                return mavsdk::rpc::camera::
                    VideoStreamInfo_VideoStreamStatus_VIDEO_STREAM_STATUS_NOT_RUNNING;
            case mav::Camera::VideoStreamInfo::VideoStreamStatus::InProgress:
                return mavsdk::rpc::camera::
                    VideoStreamInfo_VideoStreamStatus_VIDEO_STREAM_STATUS_IN_PROGRESS;
        }
    }

    static mavsdk::rpc::camera::VideoStreamInfo::VideoStreamSpectrum
    translateToRpcVideoStreamSpectrum(
        const mav::Camera::VideoStreamInfo::VideoStreamSpectrum &video_stream_spectrum) {
        switch (video_stream_spectrum) {
            default:
                base::LogError() << "Unknown video_stream_spectrum enum value: "
                                 << static_cast<int>(video_stream_spectrum);
            // FALLTHROUGH
            case mav::Camera::VideoStreamInfo::VideoStreamSpectrum::Unknown:
                return mavsdk::rpc::camera::
                    VideoStreamInfo_VideoStreamSpectrum_VIDEO_STREAM_SPECTRUM_UNKNOWN;
            case mav::Camera::VideoStreamInfo::VideoStreamSpectrum::VisibleLight:
                return mavsdk::rpc::camera::
                    VideoStreamInfo_VideoStreamSpectrum_VIDEO_STREAM_SPECTRUM_VISIBLE_LIGHT;
            case mav::Camera::VideoStreamInfo::VideoStreamSpectrum::Infrared:
                return mavsdk::rpc::camera::
                    VideoStreamInfo_VideoStreamSpectrum_VIDEO_STREAM_SPECTRUM_INFRARED;
        }
    }

    static std::unique_ptr<mavsdk::rpc::camera::VideoStreamInfo> translateToRpcVideoStreamInfo(
        const mav::Camera::VideoStreamInfo &video_stream_info) {
        auto rpc_obj = std::make_unique<mavsdk::rpc::camera::VideoStreamInfo>();

        rpc_obj->set_stream_id(video_stream_info.stream_id);

        rpc_obj->set_allocated_settings(
            translateToRpcVideoStreamSettings(video_stream_info.settings).release());

        rpc_obj->set_status(translateToRpcVideoStreamStatus(video_stream_info.status));

        rpc_obj->set_spectrum(translateToRpcVideoStreamSpectrum(video_stream_info.spectrum));

        return rpc_obj;
    }

    static std::unique_ptr<mavsdk::rpc::camera::Option> translateToRpcOption(
        const mav::Camera::Option &option) {
        auto rpc_obj = std::make_unique<mavsdk::rpc::camera::Option>();

        rpc_obj->set_option_id(option.option_id);

        rpc_obj->set_option_description(option.option_description);

        return rpc_obj;
    }

    static mav::Camera::Option translateFromRpcOption(const mavsdk::rpc::camera::Option &option) {
        mav::Camera::Option obj;

        obj.option_id = option.option_id();

        obj.option_description = option.option_description();

        return obj;
    }

    static std::unique_ptr<mavsdk::rpc::camera::Setting> translateToRpcSetting(
        const mav::Camera::Setting &setting) {
        auto rpc_obj = std::make_unique<mavsdk::rpc::camera::Setting>();

        rpc_obj->set_setting_id(setting.setting_id);

        rpc_obj->set_setting_description(setting.setting_description);

        rpc_obj->set_allocated_option(translateToRpcOption(setting.option).release());

        rpc_obj->set_is_range(setting.is_range);

        return rpc_obj;
    }

    static mav::Camera::Setting translateFromRpcSetting(
        const mavsdk::rpc::camera::Setting &setting) {
        mav::Camera::Setting obj;

        obj.setting_id = setting.setting_id();

        obj.setting_description = setting.setting_description();

        obj.option = translateFromRpcOption(setting.option());

        obj.is_range = setting.is_range();

        return obj;
    }

    static std::unique_ptr<mavsdk::rpc::camera::SettingOptions> translateToRpcSettingOptions(
        const mav::Camera::SettingOptions &setting_options) {
        auto rpc_obj = std::make_unique<mavsdk::rpc::camera::SettingOptions>();

        rpc_obj->set_setting_id(setting_options.setting_id);

        rpc_obj->set_setting_description(setting_options.setting_description);

        for (const auto &elem : setting_options.options) {
            auto *ptr = rpc_obj->add_options();
            ptr->CopyFrom(*translateToRpcOption(elem).release());
        }

        rpc_obj->set_is_range(setting_options.is_range);

        return rpc_obj;
    }

    static std::unique_ptr<mavsdk::rpc::camera::Status> translateToRpcStatus(
        const mav::Camera::Status &status) {
        auto rpc_obj = std::make_unique<mavsdk::rpc::camera::Status>();

        rpc_obj->set_video_on(status.video_on);

        rpc_obj->set_photo_interval_on(status.photo_interval_on);

        rpc_obj->set_used_storage_mib(status.used_storage_mib);

        rpc_obj->set_available_storage_mib(status.available_storage_mib);

        rpc_obj->set_total_storage_mib(status.total_storage_mib);

        rpc_obj->set_recording_time_s(status.recording_time_s);

        rpc_obj->set_media_folder_name(status.media_folder_name);

        rpc_obj->set_storage_status(translateToRpcStorageStatus(status.storage_status));

        rpc_obj->set_storage_id(status.storage_id);

        rpc_obj->set_storage_type(translateToRpcStorageType(status.storage_type));

        return rpc_obj;
    }

    static mavsdk::rpc::camera::Status::StorageStatus translateToRpcStorageStatus(
        const mav::Camera::Status::StorageStatus &storage_status) {
        switch (storage_status) {
            default:
                base::LogError() << "Unknown storage_status enum value: "
                                 << static_cast<int>(storage_status);
            // FALLTHROUGH
            case mav::Camera::Status::StorageStatus::NotAvailable:
                return mavsdk::rpc::camera::Status_StorageStatus_STORAGE_STATUS_NOT_AVAILABLE;
            case mav::Camera::Status::StorageStatus::Unformatted:
                return mavsdk::rpc::camera::Status_StorageStatus_STORAGE_STATUS_UNFORMATTED;
            case mav::Camera::Status::StorageStatus::Formatted:
                return mavsdk::rpc::camera::Status_StorageStatus_STORAGE_STATUS_FORMATTED;
            case mav::Camera::Status::StorageStatus::NotSupported:
                return mavsdk::rpc::camera::Status_StorageStatus_STORAGE_STATUS_NOT_SUPPORTED;
        }
    }

    static mavsdk::rpc::camera::Status::StorageType translateToRpcStorageType(
        const mav::Camera::Status::StorageType &storage_type) {
        switch (storage_type) {
            default:
                base::LogError() << "Unknown storage_type enum value: "
                                 << static_cast<int>(storage_type);
            // FALLTHROUGH
            case mav::Camera::Status::StorageType::Unknown:
                return mavsdk::rpc::camera::Status_StorageType_STORAGE_TYPE_UNKNOWN;
            case mav::Camera::Status::StorageType::UsbStick:
                return mavsdk::rpc::camera::Status_StorageType_STORAGE_TYPE_USB_STICK;
            case mav::Camera::Status::StorageType::Sd:
                return mavsdk::rpc::camera::Status_StorageType_STORAGE_TYPE_SD;
            case mav::Camera::Status::StorageType::Microsd:
                return mavsdk::rpc::camera::Status_StorageType_STORAGE_TYPE_MICROSD;
            case mav::Camera::Status::StorageType::Hd:
                return mavsdk::rpc::camera::Status_StorageType_STORAGE_TYPE_HD;
            case mav::Camera::Status::StorageType::Other:
                return mavsdk::rpc::camera::Status_StorageType_STORAGE_TYPE_OTHER;
        }
    }

    ::grpc::Status Prepare(::grpc::ServerContext *context,
                           const ::mavsdk::rpc::camera::PrepareRequest *request,
                           ::mavsdk::rpc::camera::PrepareResponse *response) override {
        auto result = _plugin->prepare();
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status TakePhoto(::grpc::ServerContext *context,
                             const mavsdk::rpc::camera::TakePhotoRequest *request,
                             mavsdk::rpc::camera::TakePhotoResponse *response) override {
        auto result = _plugin->take_photo();
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status StartPhotoInterval(
        ::grpc::ServerContext *context,
        const ::mavsdk::rpc::camera::StartPhotoIntervalRequest *request,
        ::mavsdk::rpc::camera::StartPhotoIntervalResponse *response) override {
        auto result = _plugin->start_photo_interval(request->interval_s());
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status StopPhotoInterval(
        ::grpc::ServerContext *context,
        const ::mavsdk::rpc::camera::StopPhotoIntervalRequest *request,
        ::mavsdk::rpc::camera::StopPhotoIntervalResponse *response) override {
        auto result = _plugin->stop_photo_interval();
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status StartVideo(::grpc::ServerContext *context,
                              const mavsdk::rpc::camera::StartVideoRequest *request,
                              mavsdk::rpc::camera::StartVideoResponse *response) override {
        auto result = _plugin->start_video();
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status StopVideo(::grpc::ServerContext *context,
                             const ::mavsdk::rpc::camera::StopVideoRequest *request,
                             ::mavsdk::rpc::camera::StopVideoResponse *response) override {
        auto result = _plugin->stop_video();
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status StartVideoStreaming(
        ::grpc::ServerContext *context,
        const ::mavsdk::rpc::camera::StartVideoStreamingRequest *request,
        ::mavsdk::rpc::camera::StartVideoStreamingResponse *response) override {
        auto result = _plugin->start_video_streaming(request->stream_id());
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status StopVideoStreaming(
        ::grpc::ServerContext *context,
        const ::mavsdk::rpc::camera::StopVideoStreamingRequest *request,
        ::mavsdk::rpc::camera::StopVideoStreamingResponse *response) override {
        auto result = _plugin->stop_video_streaming(request->stream_id());
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status SetMode(::grpc::ServerContext *context,
                           const ::mavsdk::rpc::camera::SetModeRequest *request,
                           ::mavsdk::rpc::camera::SetModeResponse *response) override {
        auto result = _plugin->set_mode(translateFromRpcMode(request->mode()));
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status ListPhotos(::grpc::ServerContext *context,
                              const ::mavsdk::rpc::camera::ListPhotosRequest *request,
                              ::mavsdk::rpc::camera::ListPhotosResponse *response) override {
        auto result = _plugin->list_photos(translateFromRpcPhotosRange(request->photos_range()));
        if (response != nullptr) {
            fillResponseWithResult(response, result.first);
        }
        return ::grpc::Status::OK;
    }

    ::grpc::Status SubscribePossibleSettingOptions(
        ::grpc::ServerContext *context,
        const ::mavsdk::rpc::camera::SubscribePossibleSettingOptionsRequest *request,
        ::grpc::ServerWriter<::mavsdk::rpc::camera::PossibleSettingOptionsResponse> *writer)
        override {
        return ::grpc::Status::OK;
    }

    ::grpc::Status SetSetting(::grpc::ServerContext *context,
                              const ::mavsdk::rpc::camera::SetSettingRequest *request,
                              ::mavsdk::rpc::camera::SetSettingResponse *response) override {
        auto result = _plugin->set_setting(translateFromRpcSetting(request->setting()));

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return grpc::Status::OK;
    }

    ::grpc::Status GetSetting(::grpc::ServerContext *context,
                              const ::mavsdk::rpc::camera::GetSettingRequest *request,
                              ::mavsdk::rpc::camera::GetSettingResponse *response) override {
        auto result = _plugin->get_setting(translateFromRpcSetting(request->setting()));

        if (response != nullptr) {
            fillResponseWithResult(response, result.first);
            response->set_allocated_setting(translateToRpcSetting(result.second).release());
        }

        return ::grpc::Status::OK;
    }

    ::grpc::Status FormatStorage(::grpc::ServerContext *context,
                                 const ::mavsdk::rpc::camera::FormatStorageRequest *request,
                                 ::mavsdk::rpc::camera::FormatStorageResponse *response) override {
        auto result = _plugin->format_storage(request->storage_id());

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return ::grpc::Status::OK;
    }

    ::grpc::Status SelectCamera(::grpc::ServerContext *context,
                                const ::mavsdk::rpc::camera::SelectCameraRequest *request,
                                ::mavsdk::rpc::camera::SelectCameraResponse *response) override {
        auto result = _plugin->select_camera(request->camera_id());

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return ::grpc::Status::OK;
    }

    ::grpc::Status ResetSettings(::grpc::ServerContext *context,
                                 const ::mavsdk::rpc::camera::ResetSettingsRequest *request,
                                 ::mavsdk::rpc::camera::ResetSettingsResponse *response) override {
        auto result = _plugin->reset_settings();

        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return ::grpc::Status::OK;
    }

    ::grpc::Status SetDefinitionData(
        ::grpc::ServerContext *context,
        const ::mavsdk::rpc::camera::SetDefinitionDataRequest *request,
        ::mavsdk::rpc::camera::SetDefinitionDataResponse *response) override {
        auto result = _plugin->set_definition_data(request->definition_data());
        if (response != nullptr) {
            fillResponseWithResult(response, result);
        }

        return ::grpc::Status::OK;
    }

    void stop() {
        _stopped.store(true);
        for (auto &prom : _stream_stop_promises) {
            if (auto handle = prom.lock()) {
                handle->set_value();
            }
        }
    }
private:
    void register_stream_stop_promise(std::weak_ptr<std::promise<void>> prom) {
        // If we have already stopped, set promise immediately and don't add it to list.
        if (_stopped.load()) {
            if (auto handle = prom.lock()) {
                handle->set_value();
            }
        } else {
            _stream_stop_promises.push_back(prom);
        }
    }

    void unregister_stream_stop_promise(std::shared_ptr<std::promise<void>> prom) {
        for (auto it = _stream_stop_promises.begin(); it != _stream_stop_promises.end();
             /* ++it */) {
            if (it->lock() == prom) {
                it = _stream_stop_promises.erase(it);
            } else {
                ++it;
            }
        }
    }
private:
    std::shared_ptr<Camera> _plugin;
    std::atomic<bool> _stopped{false};
    std::vector<std::weak_ptr<std::promise<void>>> _stream_stop_promises{};
};

}  // namespace mav