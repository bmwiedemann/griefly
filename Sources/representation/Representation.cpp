#include "Representation.h"

#include "constheader.h"
#include "helpers.h"

void Representation::Process()
{
    // TODO: mutex
    // Lock(mutex_);

    SynchronizeViews();

    Draw();

    const int PIXEL_MOVEMENT_SPEED = 16;

    if ((SDL_GetTicks() - pixel_movement_tick_) > PIXEL_MOVEMENT_SPEED)
    {
        PerformPixelMovement();
        camera_.PerformPixelMovement();
        pixel_movement_tick_ = SDL_GetTicks();
    }
}

void Representation::SynchronizeViews()
{
    if (is_updated_)
    {
        camera_.SetPos(current_frame_->camera_pos_x, current_frame_->camera_pos_y);

        for (auto it = current_frame_->entities.begin(); it != current_frame_->entities.end(); ++it)
        {
            auto& view_with_frame_id = views_[it->id];
            view_with_frame_id.view.LoadViewInfo(it->view);
            if (view_with_frame_id.frame_id != current_frame_id_)
            {
                view_with_frame_id.view.SetX(it->pos_x * 32);
                view_with_frame_id.view.SetY(it->pos_y * 32);
            }
            view_with_frame_id.frame_id = current_frame_id_ + 1;
        } 

        ++current_frame_id_;
        is_updated_ = false;
    }
}

int get_pixel_speed_for_distance(int distance)
{
    int sign = 0;
    if (distance > 0)
    {
        sign = 1;
    }
    if (distance < 0)
    {
        sign = -1;
    }

    if (sign == 0)
    {
        return 0;
    }

    distance = std::abs(distance);

    return sign * (((distance - 1) / 8) + 1);
}

void Representation::PerformPixelMovement()
{
    for (auto it = current_frame_->entities.begin(); it != current_frame_->entities.end(); ++it)
    {
        int pixel_x = it->pos_x * 32;
        int pixel_y = it->pos_y * 32;

        auto& view_with_frame_id = views_[it->id];
        int old_x = view_with_frame_id.view.GetX();
        int old_y = view_with_frame_id.view.GetY();
        if (old_x != pixel_x)
        {
            int diff_x = pixel_x - old_x;
            diff_x = get_pixel_speed_for_distance(diff_x);
            view_with_frame_id.view.SetX(old_x + diff_x);
        }
        if (old_y != pixel_y)
        {
            int diff_y = pixel_y - old_y;
            diff_y = get_pixel_speed_for_distance(diff_y);
            view_with_frame_id.view.SetY(old_y + diff_y);
        }
    }
}

void Representation::Draw()
{
    for (int vlevel = 0; vlevel < MAX_LEVEL; ++vlevel)
    {
        for (auto it = current_frame_->entities.begin(); it != current_frame_->entities.end(); ++it)
        {
            if (it->vlevel == vlevel)
            {
                views_[it->id].view.Draw(
                    camera_.GetFullShiftX(),
                    camera_.GetFullShiftY(),
                    helpers::dir_to_byond(it->dir));
            }
        }
    }
    for (auto it = current_frame_->entities.begin(); it != current_frame_->entities.end(); ++it)
    {
        if (it->vlevel >= MAX_LEVEL)
        {
            views_[it->id].view.Draw(
                camera_.GetFullShiftX(),
                camera_.GetFullShiftY(),
                helpers::dir_to_byond(it->dir));
        }
    }
}



Representation::Camera::Camera()
{
    pos_x = 0;
    pos_y = 0;
    pixel_shift_x_ = 0;
    pixel_shift_y_ = 0;
}

void Representation::Camera::SetPos(int new_pos_x, int new_pos_y)
{
    pixel_shift_x_ += (pos_x - new_pos_x) * 32;
    pixel_shift_y_ += (pos_y - new_pos_y) * 32;

    pos_x = new_pos_x;
    pos_y = new_pos_y;
}

void Representation::Camera::PerformPixelMovement()
{
    if (pixel_shift_x_ != 0)
    {
        int diff_x = get_pixel_speed_for_distance(pixel_shift_x_);
        pixel_shift_x_ -= diff_x;
    }
    if (pixel_shift_y_ != 0)
    {
        int diff_y = get_pixel_speed_for_distance(pixel_shift_y_);
        pixel_shift_y_ -= diff_y;
    }
}

int Representation::Camera::GetFullShiftX()
{
    return -1 * (pos_x * 32 + pixel_shift_x_) + (sizeW / 2) - 16;
}

int Representation::Camera::GetFullShiftY()
{
    return -1 * (pos_y * 32 + pixel_shift_y_) + (sizeH / 2) - 16;
}

Representation* g_r = nullptr;
Representation& GetRepresentation()
{
    return *g_r;
}

void SetRepresentation(Representation* new_g_r)
{
    g_r = new_g_r;
}
