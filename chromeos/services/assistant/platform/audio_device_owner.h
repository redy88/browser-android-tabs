// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_SERVICES_ASSISTANT_PLATFORM_AUDIO_DEVICE_OWNER_H_
#define CHROMEOS_SERVICES_ASSISTANT_PLATFORM_AUDIO_DEVICE_OWNER_H_

#include <memory>
#include <string>
#include <vector>

#include "base/component_export.h"
#include "base/macros.h"
#include "libassistant/shared/public/platform_audio_output.h"
#include "media/base/audio_block_fifo.h"
#include "media/base/audio_parameters.h"
#include "media/base/audio_renderer_sink.h"
#include "services/audio/public/cpp/output_device.h"
#include "services/service_manager/public/cpp/connector.h"

namespace chromeos {
namespace assistant {

class COMPONENT_EXPORT(ASSISTANT_SERVICE) AudioDeviceOwner
    : public media::AudioRendererSink::RenderCallback {
 public:
  AudioDeviceOwner(
      scoped_refptr<base::SequencedTaskRunner> task_runner,
      scoped_refptr<base::SequencedTaskRunner> background_task_runner,
      const std::string& device_id);
  ~AudioDeviceOwner() override;

  void StartOnMainThread(assistant_client::AudioOutput::Delegate* delegate,
                         service_manager::Connector* connector,
                         const assistant_client::OutputStreamFormat& format);

  void StopOnBackgroundThread();

  // media::AudioRenderSink::RenderCallback overrides:
  int Render(base::TimeDelta delay,
             base::TimeTicks delay_timestamp,
             int prior_frames_skipped,
             media::AudioBus* dest) override;

  void OnRenderError() override;

  void SetDelegate(assistant_client::AudioOutput::Delegate* delegate);

 private:
  void StartDeviceOnBackgroundThread(
      std::unique_ptr<service_manager::Connector> connector);

  // Requests assistant to fill buffer with more data.
  void ScheduleFillLocked(const base::TimeTicks& time);

  // Callback for assistant to notify that it completes the filling.
  void BufferFillDone(int num_bytes);

  scoped_refptr<base::SequencedTaskRunner> main_task_runner_;
  scoped_refptr<base::SequencedTaskRunner> background_task_runner_;

  base::Lock lock_;
  std::unique_ptr<media::AudioBlockFifo> audio_fifo_;  // guarded by lock_.
  // Whether assistant is filling the buffer -- delegate_->FillBuffer is called
  // and BufferFillDone() is not called yet.
  // guarded by lock_.
  bool is_filling_ = false;

  assistant_client::AudioOutput::Delegate* delegate_;

  // Audio output device id used for output.
  std::string device_id_;
  std::unique_ptr<audio::OutputDevice> output_device_;
  // Stores audio frames generated by assistant.
  std::vector<uint8_t> audio_data_;
  assistant_client::OutputStreamFormat format_;
  media::AudioParameters audio_param_;

  DISALLOW_COPY_AND_ASSIGN(AudioDeviceOwner);
};

}  // namespace assistant
}  // namespace chromeos

#endif  // CHROMEOS_SERVICES_ASSISTANT_PLATFORM_AUDIO_DEVICE_OWNER_H_
