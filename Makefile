###########################################################################
#
# APX project makefile
#
# make <subdir> <target>
###########################################################################
include Rules.mk

SUBDIRS := gcs docker tools

$(SUBDIRS): FORCE
	@make -C $@ $(filter-out $(SUBDIRS),$(MAKECMDGOALS))



release-request: commit
	@echo "Requesting release..."
	@git tag release-$$($(GIT_VERSION_CMD))
	@git push origin $(GIT_BRANCH)
	@git push origin $(GIT_BRANCH) --tags
	@git fetch . $(GIT_BRANCH):release -f
	@git push origin release -f

docker-release-request: commit
	@echo "Requesting docker images release..."
	@git fetch . $(GIT_BRANCH):docker -f
	@git push origin docker -f

docs-release-request:
	@echo "Requesting docs release..."
	@make -C docs timestamp
	@make commit
	
test-request: commit
	@echo "Requesting build test..."
	@git fetch . $(GIT_BRANCH):test -f
	@git push origin test -f



commit: FORCE
	@git add -A
	@if git diff-index --quiet HEAD --; then echo "No changes"; else git commit -a; fi
	@git push origin $(GIT_BRANCH)

upload-assets: FORCE
	@python $(TOOLS_DIR)/release/make_release.py $(GITHUB_TOKEN:%=--token=%) --assets=$(BUILD_DIR_OUT)

publish-release: FORCE
	@python $(TOOLS_DIR)/release/make_release.py $(GITHUB_TOKEN:%=--token=%) --publish
	@python $(TOOLS_DIR)/release/make_announcement.py $(GITHUB_TOKEN:%=--token=%)

changes:
	@python $(TOOLS_DIR)/release/make_release.py --changes



# export-gpg-key:
# 	gpg --export-secret-key $(GPG_KEY) >~/gpg-keyring.gpg
# 	travis encrypt-file ~/gpg-keyring.gpg --com
# 	rm -f ~/gpg-keyring.gpg
# 	mv gpg-keyring.gpg.enc tools/gpg-keyring.gpg.enc






# subdir target catch
%:
	@echo "Finished: $*"
#
