#include "jwt.hpp"
#include "resource.h"

using namespace jwt;

namespace {

  struct TrackBarTest {
    TrackBarTest()
      : d_(IDD_TRACKBARTESTS), 
        t1_(d_, IDC_SLIDER1), t2_(d_, IDC_SLIDER2),
        b1_(d_, IDC_PROGRESS1), b2_(d_, IDC_PROGRESS2), b3_(d_, IDC_PROGRESS3)
    {
      d_.On(Close, [this]() {
        SetVisible(d_, false);
      });

      t1_.On(Change, [this]() {
        SetValue(b1_, GetValue(t1_));
      });

      AddStyle(b3_, PBS_MARQUEE);
      SetMarquee(b3_, true);
    }

    void Show() {
      SetVisible(d_, true);
    }

  private:
    Dialog d_;

    TrackBar t1_, t2_;
    ProgressBar b1_, b2_, b3_;

  } trackBarTest;

}

void ShowTrackBarTests() {
  trackBarTest.Show();
}