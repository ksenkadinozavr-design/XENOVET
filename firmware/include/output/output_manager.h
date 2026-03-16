#pragma once

namespace xenovent::output {

enum class FeedbackPattern {
  Confirm,
  Error,
  Warning,
  Mutation,
  Death,
};

class OutputManager {
 public:
  void begin();
  void play(FeedbackPattern pattern);
};

}  // namespace xenovent::output
