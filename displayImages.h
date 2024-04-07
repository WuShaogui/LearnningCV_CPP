#include <string>
#include <fstream>
#include <opencv2/imgcodecs.hpp>
#include "xtl/xbase64.hpp"
#include "xeus/xjson.hpp"
#include <xcpp/xdisplay.hpp>

namespace im
{
    struct FileImage
    {   
        FileImage(const std::string& filename)
        {
            std::ifstream fin(filename, std::ios::binary);   
            m_buffer << fin.rdbuf();
        }
        
        std::stringstream m_buffer;
    };

    struct MatImage
    {
        MatImage(const cv::Mat& m) : _mat(m) {}
        cv::Mat _mat;
    };

    inline auto show(const std::string& filename)
    {
        return im::FileImage(filename);
    }

    inline auto show(const cv::Mat& img)
    {
        return im::MatImage(img);
    }
    
    xeus::xjson mime_bundle_repr(const FileImage& img)
    {
        auto bundle = xeus::xjson::object();
        bundle["image/png"] = xtl::base64encode(img.m_buffer.str());
        return bundle;
    }

    xeus::xjson mime_bundle_repr(const MatImage& img)
    {
        std::vector<uchar> buf;
        bool success = cv::imencode(".png", img._mat, buf);
        if (success)
        {
            std::string bufString(buf.begin(), buf.end());
            auto bundle = xeus::xjson::object();
            bundle["image/png"] = xtl::base64encode(bufString);
            //bundle["text/plain"] = xtl::base64encode(bufString);
            return bundle;
        }
        else
        {
            return {};
        }
    }
}

void displayImage(const cv::Mat& image){
    xcpp::display(im::show(image));
}

void displayImage(const std::string& fileName){
    xcpp::display(im::show(fileName));
}
