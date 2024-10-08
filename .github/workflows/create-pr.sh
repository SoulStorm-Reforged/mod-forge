# Adapted from https://github.com/paygoc6/action-pull-request-another-repo

CLONE_DIR=$(mktemp -d)

echo "Setting git variables"
export GITHUB_TOKEN=$API_TOKEN_GITHUB
git config --global user.email "$USER_EMAIL"
git config --global user.name "$USER_NAME"

date=$(date '+%Y-%m-%d_%H-%M')
DESTINATION_HEAD_BRANCH="$DESTINATION_HEAD_BRANCH-$date"

echo "Cloning destination git repository"
git clone "https://$API_TOKEN_GITHUB@github.com/$DESTINATION_REPO.git" "$CLONE_DIR"
cd "$CLONE_DIR"
git checkout "$DESTINATION_BASE_BRANCH"
git pull origin "$DESTINATION_BASE_BRANCH"
git checkout -b "$DESTINATION_HEAD_BRANCH"

echo "Copying contents to git repo"
mkdir -p "$CLONE_DIR/$DESTINATION_FOLDER"
cp -r "$SOURCE_FOLDER/." "$CLONE_DIR/$DESTINATION_FOLDER/"

echo "Adding files"
git add .
echo "Git status:"
git status -- ":!pages/forge/date.js"
if git status -- ":!pages/forge/date.js" | grep -q "Changes to be committed"
then
  echo "Adding git commit"
  git commit -m "$COMMIT_MESSAGE"
  echo "Pushing git commit"
  git push -u origin "$DESTINATION_HEAD_BRANCH"
  echo "Creating a pull request"
  gh pr create -t "$PR_TITLE" \
               -B "$DESTINATION_BASE_BRANCH" \
               -b "" \
               -H "$DESTINATION_HEAD_BRANCH"
else
  echo "No changes detected"
fi
