#include "editions/edition_defaults_test_utils.h"

#include "google/protobuf/descriptor.pb.h"
#include <gmock/gmock.h>
#include "absl/types/optional.h"

namespace google {
namespace protobuf {
namespace compiler {

absl::optional<FeatureSetDefaults::FeatureSetEditionDefault> FindEditionDefault(
    const FeatureSetDefaults& defaults, Edition edition) {
  for (const auto& edition_default : defaults.defaults()) {
    if (edition_default.edition() == edition) {
      return edition_default;
    }
  }
  return absl::nullopt;
}

void ScrubUnstable(FeatureSetDefaults& defaults) {
  auto* mutable_defaults = defaults.mutable_defaults();
  for (int i = 0; i < mutable_defaults->size(); ++i) {
    if (mutable_defaults->Get(i).edition() == EDITION_UNSTABLE) {
      mutable_defaults->DeleteSubrange(i, 1);
      --i;
    }
  }
}

void CheckEditionDefaults(const FeatureSetDefaults& actual,
                          const FeatureSetDefaults& expected) {
  FeatureSetDefaults scrubbed_actual = actual;
  ScrubUnstable(scrubbed_actual);
#ifdef PROTOBUF_INTERNAL_IMPL
  EXPECT_THAT(
      scrubbed_actual,
      ::testing::proto::IgnoringRepeatedFieldOrdering(
          ::testing::proto::Partially(::testing::EqualsProto(expected))));
#else
  EXPECT_THAT(scrubbed_actual, ::testing::EqualsProto(expected));
#endif
}

}  // namespace compiler
}  // namespace protobuf
}  // namespace google
