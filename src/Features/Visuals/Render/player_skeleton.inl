if (g::visualsSkeleton) {
    const ImU32 skeletonRenderCol = g::visualsVisibilityColoring ? entityCol : skelCol;
    const int leftToeBoneId = selectToeBoneId(p, true);
    const int rightToeBoneId = selectToeBoneId(p, false);
    const float skelInner = 1.6f * g::visualsThickness;
    const float skelOuter = skelInner + 1.4f;
    if (canRenderRealSkeleton) {
        for (int b = 0; b < skeletonPairCount; b++) {
            const int from = skeletonPairs[b].from;
            const int to = skeletonPairs[b].to;
            if (!boneScreenValid[from] || !boneScreenValid[to]) continue;
            const ImVec2 p1(boneScreen[from].x, boneScreen[from].y);
            const ImVec2 p2(boneScreen[to].x, boneScreen[to].y);
            drawList->AddLine(p1, p2, IM_COL32(0, 0, 0, 180), skelOuter);
            drawList->AddLine(p1, p2, skeletonRenderCol, skelInner);
        }
        if (hasLeftToeBone && boneScreenValid[visuals::FOOT_HEEL_L] && boneScreenValid[leftToeBoneId]) {
            const ImVec2 p1(boneScreen[visuals::FOOT_HEEL_L].x, boneScreen[visuals::FOOT_HEEL_L].y);
            const ImVec2 p2(boneScreen[leftToeBoneId].x, boneScreen[leftToeBoneId].y);
            drawList->AddLine(p1, p2, IM_COL32(0, 0, 0, 180), skelOuter);
            drawList->AddLine(p1, p2, skeletonRenderCol, skelInner);
        }
        if (hasRightToeBone && boneScreenValid[visuals::FOOT_HEEL_R] && boneScreenValid[rightToeBoneId]) {
            const ImVec2 p1(boneScreen[visuals::FOOT_HEEL_R].x, boneScreen[visuals::FOOT_HEEL_R].y);
            const ImVec2 p2(boneScreen[rightToeBoneId].x, boneScreen[rightToeBoneId].y);
            drawList->AddLine(p1, p2, IM_COL32(0, 0, 0, 180), skelOuter);
            drawList->AddLine(p1, p2, skeletonRenderCol, skelInner);
        }

        if (g::visualsSkeletonDots) {
            const int jointBones[] = {
                visuals::PELVIS,
                visuals::SPINE1,
                visuals::SPINE2,
                visuals::CHEST,
                visuals::NECK,
                visuals::SHOULDER_L,
                visuals::ELBOW_L,
                visuals::HAND_L,
                visuals::SHOULDER_R,
                visuals::ELBOW_R,
                visuals::HAND_R,
                visuals::HIP_L,
                visuals::KNEE_L,
                visuals::FOOT_HEEL_L,
                visuals::HIP_R,
                visuals::KNEE_R,
                visuals::FOOT_HEEL_R,
                visuals::HEAD
            };
            for (int jIdx = 0; jIdx < static_cast<int>(sizeof(jointBones) / sizeof(jointBones[0])); ++jIdx) {
                const int b = jointBones[jIdx];
                if (!boneScreenValid[b]) continue;
                const ImVec2 jp(boneScreen[b].x, boneScreen[b].y);
                drawList->AddCircleFilled(jp, 2.2f, IM_COL32(0, 0, 0, 180), 6);
                drawList->AddCircleFilled(jp, 1.4f, skeletonRenderCol, 6);
            }
            if (hasLeftToeBone && boneScreenValid[leftToeBoneId]) {
                const ImVec2 jp(boneScreen[leftToeBoneId].x, boneScreen[leftToeBoneId].y);
                drawList->AddCircleFilled(jp, 2.2f, IM_COL32(0, 0, 0, 180), 6);
                drawList->AddCircleFilled(jp, 1.4f, skeletonRenderCol, 6);
            }
            if (hasRightToeBone && boneScreenValid[rightToeBoneId]) {
                const ImVec2 jp(boneScreen[rightToeBoneId].x, boneScreen[rightToeBoneId].y);
                drawList->AddCircleFilled(jp, 2.2f, IM_COL32(0, 0, 0, 180), 6);
                drawList->AddCircleFilled(jp, 1.4f, skeletonRenderCol, 6);
            }
        }
    }
}
